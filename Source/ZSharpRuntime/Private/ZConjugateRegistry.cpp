// Copyright Zero Games. All Rights Reserved.


#include "ZConjugateRegistry.h"

#include "ZSharpRuntimeLogChannels.h"
#include "CLR/IZSharpCLR.h"
#include "ALC/IZAssembly.h"
#include "ALC/IZMasterAssemblyLoadContext.h"

namespace ZSharp::FZConjugateRegistry_Private
{
	template <typename T>
	void ReleaseConjugates(FZConjugateRegistry& registry, FZConjugateRegistry::TZConjugateSubregistry<T>& subregistry, bool bCapturedOnly)
	{
		TArray<T*> pendings;
		for (const auto& pair : subregistry)
		{
			if (bCapturedOnly && pair.Value.bCaptured)
			{
				pendings.Emplace(pair.Key);
			}
		}

		for (const auto& unmanaged : pendings)
		{
			registry.ReleaseConjugate(unmanaged);
		}
	}
}

TUniquePtr<ZSharp::FZConjugateRegistry> ZSharp::FZConjugateRegistry::GSingleton;

void ZSharp::FZConjugateRegistry::Startup()
{
	UE_CLOG(!!IZSharpCLR::Get().GetMasterALC(), LogZSharpRuntime, Fatal, TEXT("Conjugate Registry must startup before any master ALC!!!"));
	
	IZSharpCLR::Get().RegisterMasterALCLoaded(FZOnMasterALCLoaded::FDelegate::CreateStatic(&ThisClass::HandleMasterALCLoaded));
	IZSharpCLR::Get().RegisterMasterALCUnloaded(FZOnMasterALCUnloaded::FDelegate::CreateStatic(&ThisClass::HandleMasterALCUnloaded));
}

ZSharp::FZConjugateRegistry* ZSharp::FZConjugateRegistry::Get()
{
	return GSingleton.Get();
}

ZSharp::FZConjugateRegistry::FZConjugateRegistry(IZMasterAssemblyLoadContext* masterALC)
	: MasterALC(masterALC)
	, ZCallToManagedDepth(0)
{
	MasterALC->RegisterPreZCallToManaged(FZPreZCallToManaged::FDelegate::CreateRaw(this, &ThisClass::HandlePreZCallToManaged));
	MasterALC->RegisterPostZCallToManaged(FZPostZCallToManaged::FDelegate::CreateRaw(this, &ThisClass::HandlePostZCallToManaged));
	FCoreUObjectDelegates::GarbageCollectComplete.AddRaw(this, &ThisClass::HandleGarbageCollectComplete);
}

ZSharp::FZConjugateRegistry::~FZConjugateRegistry()
{
	FCoreUObjectDelegates::GarbageCollectComplete.RemoveAll(this);
	// We have no need to remove ALC delegates because it's pending kill.
}

ZSharp::FZConjugateHandle ZSharp::FZConjugateRegistry::Conjugate(UObject* unmanaged)
{
	CheckGuarded();
	
	FZConjugateHandle handle = MasterALC->Conjugate(unmanaged);
	if (IsValid(handle))
	{
		return handle;
	}

	UClass* unmanagedMostDerivedNativeClass = unmanaged->GetClass();
	while (!unmanagedMostDerivedNativeClass->HasAnyClassFlags(CLASS_Native | CLASS_Intrinsic))
	{
		unmanagedMostDerivedNativeClass = unmanagedMostDerivedNativeClass->GetSuperClass();
	}

	FString asmName = "ZeroGames.ZSharp.UnrealEngine";
	FString managedTypeName = "ZeroGames.ZSharp.UnrealEngine.CoreUObject.UnrealObject";
	const IZType* managedType = MasterALC->GetAssembly(asmName)->GetType(managedTypeName);
	
	if (!managedType)
	{
		return FZConjugateHandle{};
	}
	
	handle = MasterALC->BuildConjugate(unmanaged, managedType);
	if (IsValid(handle))
	{
		ObjectRegistry.Emplace(unmanaged, unmanaged);
	}

	return handle;
}

void ZSharp::FZConjugateRegistry::ReleaseConjugate(UObject* unmanaged)
{
	CheckNoGuarded();
	
	if (!ObjectRegistry.Remove(unmanaged))
	{
		return;
	}

	MasterALC->ReleaseConjugate(unmanaged);
}

ZSharp::FZConjugateHandle ZSharp::FZConjugateRegistry::Conjugate(FString* unmanaged, const TFunction<void(FString*)>& onReleased)
{
	CheckGuarded();
	
	FZConjugateHandle handle = MasterALC->Conjugate(unmanaged);
	if (IsValid(handle))
	{
		return handle;
	}

	FString asmName = "ZeroGames.ZSharp.UnrealEngine";
	FString managedTypeName = "ZeroGames.ZSharp.UnrealEngine.Core.UnrealString";
	const IZType* managedType = MasterALC->GetAssembly(asmName)->GetType(managedTypeName);
	
	if (!managedType)
	{
		return FZConjugateHandle{};
	}
	
	handle = MasterALC->BuildConjugate(unmanaged, managedType);
	if (IsValid(handle))
	{
		StringRegistry.Emplace(unmanaged, { unmanaged, true, onReleased });
	}

	return handle;
}

void ZSharp::FZConjugateRegistry::Conjugate(FString* unmanaged, FZConjugateHandle managed, const TFunction<void(FString*)>& onReleased)
{
	CheckGuarded();
	
	if (!IsValid(managed))
	{
		return;
	}

	MasterALC->BuildConjugate(unmanaged, managed);
	StringRegistry.Emplace(unmanaged, { unmanaged, false, onReleased ? onReleased : [](FString* unmanaged)
	{
		UE_LOG(LogZSharpRuntime, Log, TEXT("Release UnrealString: [%s] [%p]"), **unmanaged, unmanaged);
		delete unmanaged;
	}});
}

void ZSharp::FZConjugateRegistry::ReleaseConjugate(FString* unmanaged)
{
	CheckNoGuarded();

	TZConjugateRecord<FString> rec;
	if (!StringRegistry.RemoveAndCopyValue(unmanaged, rec))
	{
		return;
	}

	if (rec.OnReleased)
	{
		rec.OnReleased(rec.Unmanaged);
	}

	MasterALC->ReleaseConjugate(unmanaged);
}

ZSharp::FZConjugateHandle ZSharp::FZConjugateRegistry::Conjugate(FZTypedScriptStruct* unmanaged, const TFunction<void(FZTypedScriptStruct*)>& onReleased)
{
	CheckGuarded();
	
	checkNoEntry();
	return FZConjugateHandle{};
}

void ZSharp::FZConjugateRegistry::Conjugate(FZTypedScriptStruct* unmanaged, FZConjugateHandle managed, const TFunction<void(FZTypedScriptStruct*)>& onReleased)
{
	CheckGuarded();
	
	checkNoEntry();
}

void ZSharp::FZConjugateRegistry::ReleaseConjugate(FZTypedScriptStruct* unmanaged)
{
	CheckNoGuarded();
	
	checkNoEntry();
}

void* ZSharp::FZConjugateRegistry::Conjugate(FZConjugateHandle managed) const
{
	return MasterALC->Conjugate(managed);
}

void ZSharp::FZConjugateRegistry::CheckGuarded() const
{
	UE_CLOG(!IsGuarded(), LogZSharpRuntime, Fatal, TEXT("Conjugate guard check failed!!!"));
}

void ZSharp::FZConjugateRegistry::CheckNoGuarded() const
{
	UE_CLOG(IsGuarded(), LogZSharpRuntime, Fatal, TEXT("Conjugate guard check failed!!!"));
}

void ZSharp::FZConjugateRegistry::HandlePreZCallToManaged()
{
	++ZCallToManagedDepth;
}

void ZSharp::FZConjugateRegistry::HandlePostZCallToManaged()
{
	CheckGuarded();
	
	if (!--ZCallToManagedDepth)
	{
		GuardRelease();
	}
}

void ZSharp::FZConjugateRegistry::HandleGarbageCollectComplete()
{
	TArray<FObjectKey> pendingRemoves;
	for (const auto& pair : ObjectRegistry)
	{
		if (!pair.Key.ResolveObjectPtrEvenIfGarbage())
		{
			pendingRemoves.Emplace(pair.Key);
		}
	}

	for (const auto& key : pendingRemoves)
	{
		UObject* unmanaged;
		ObjectRegistry.RemoveAndCopyValue(key, unmanaged);
		MasterALC->ReleaseConjugate(unmanaged);
	}
}

void ZSharp::FZConjugateRegistry::HandleMasterALCLoaded(IZMasterAssemblyLoadContext* alc)
{
	check(!GSingleton);

	GSingleton = MakeUnique<FZConjugateRegistry>(alc);
}

void ZSharp::FZConjugateRegistry::HandleMasterALCUnloaded()
{
	check(GSingleton);

	GSingleton->ReleaseConjugates(false);

	GSingleton = nullptr;
}

void ZSharp::FZConjugateRegistry::ReleaseConjugates(bool bCapturedOnly)
{
	FZConjugateRegistry_Private::ReleaseConjugates(*this, StringRegistry, bCapturedOnly);
}

void ZSharp::FZConjugateRegistry::GuardRelease()
{
	ReleaseConjugates(true);
}


