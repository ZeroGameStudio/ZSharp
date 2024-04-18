// Copyright Zero Games. All Rights Reserved.


#include "ZConjugateRegistry.h"

#include "CLR/IZSharpCLR.h"
#include "Interop/IZAssembly.h"
#include "Interop/IZMasterAssemblyLoadContext.h"

ZSharp::FZConjugateRegistry& ZSharp::FZConjugateRegistry::Get()
{
	static FZConjugateRegistry GSingleton;
	
	return GSingleton;
}

ZSharp::FZConjugateRegistry::FZConjugateRegistry()
{
	IZSharpCLR::Get().RegisterMasterALCLoaded(FZOnMasterALCLoaded::FDelegate::CreateRaw(this, &FZConjugateRegistry::HandleMasterALCLoaded));
	IZSharpCLR::Get().RegisterMasterALCUnloaded(FZOnMasterALCUnloaded::FDelegate::CreateRaw(this, &FZConjugateRegistry::HandleMasterALCUnloaded));
}

ZSharp::FZConjugateHandle ZSharp::FZConjugateRegistry::BuildConjugate(UObject* unmanaged)
{
	IZMasterAssemblyLoadContext* alc = IZSharpCLR::Get().GetMasterALC();
	if (!alc)
	{
		return FZConjugateHandle{};
	}

	FZConjugateHandle handle = alc->Conjugate(unmanaged);
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
	FString managedTypeName = "ZeroGames.ZSharp.UnrealEngine.CoreUObject.Object";
	const IZType* managedType = alc->GetAssembly(asmName)->GetType(managedTypeName);
	
	if (!managedType)
	{
		return FZConjugateHandle{};
	}
	
	handle = alc->BuildConjugate(unmanaged, managedType);
	if (IsValid(handle))
	{
		ObjectRegistry.Emplace(unmanaged, unmanaged);
	}

	return handle;
}

void ZSharp::FZConjugateRegistry::ReleaseConjugate(UObject* unmanaged)
{
	if (!ObjectRegistry.Remove(unmanaged))
	{
		return;
	}
	
	IZMasterAssemblyLoadContext* alc = IZSharpCLR::Get().GetMasterALC();
	if (!alc)
	{
		return;
	}
	
	alc->ReleaseConjugate(unmanaged);
}

ZSharp::FZConjugateHandle ZSharp::FZConjugateRegistry::BuildConjugate(FString* unmanaged)
{
	IZMasterAssemblyLoadContext* alc = IZSharpCLR::Get().GetMasterALC();
	if (!alc)
	{
		return FZConjugateHandle{};
	}

	FZConjugateHandle handle = alc->Conjugate(unmanaged);
	if (IsValid(handle))
	{
		return handle;
	}

	FString asmName = "ZeroGames.ZSharp.UnrealEngine";
	FString managedTypeName = "ZeroGames.ZSharp.UnrealEngine.Core.String";
	const IZType* managedType = alc->GetAssembly(asmName)->GetType(managedTypeName);
	
	if (!managedType)
	{
		return FZConjugateHandle{};
	}
	
	handle = alc->BuildConjugate(unmanaged, managedType);
	if (IsValid(handle))
	{
		StringRegistry.Emplace(unmanaged);
	}

	return handle;
}

void ZSharp::FZConjugateRegistry::BuildConjugate(FString* unmanaged, FZConjugateHandle managed)
{
	if (!IsValid(managed))
	{
		return;
	}
	
	IZMasterAssemblyLoadContext* alc = IZSharpCLR::Get().GetMasterALC();
	if (!alc)
	{
		return;
	}

	alc->BuildConjugate(unmanaged, managed);
	StringRegistry.Emplace(unmanaged);
}

void ZSharp::FZConjugateRegistry::ReleaseConjugate(FString* unmanaged)
{
	if (!StringRegistry.Remove(unmanaged))
	{
		return;
	}
	
	IZMasterAssemblyLoadContext* alc = IZSharpCLR::Get().GetMasterALC();
	if (!alc)
	{
		return;
	}
	
	alc->ReleaseConjugate(unmanaged);
}

ZSharp::FZConjugateHandle ZSharp::FZConjugateRegistry::BuildConjugate(FZTypedScriptStruct* unmanaged)
{
	checkNoEntry();
	return FZConjugateHandle{};
}

void ZSharp::FZConjugateRegistry::BuildConjugate(FZTypedScriptStruct* unmanaged, FZConjugateHandle managed)
{
	checkNoEntry();
}

void ZSharp::FZConjugateRegistry::ReleaseConjugate(FZTypedScriptStruct* unmanaged)
{
	checkNoEntry();
}

ZSharp::FZConjugateHandle ZSharp::FZConjugateRegistry::InternalConjugate(void* unmanaged) const
{
	IZMasterAssemblyLoadContext* alc = IZSharpCLR::Get().GetMasterALC();
	if (!alc)
	{
		return FZConjugateHandle{};
	}

	return alc->Conjugate(unmanaged);
}

void* ZSharp::FZConjugateRegistry::InternalConjugate(FZConjugateHandle managed) const
{
	IZMasterAssemblyLoadContext* alc = IZSharpCLR::Get().GetMasterALC();
	if (!alc)
	{
		return nullptr;
	}

	return alc->Conjugate(managed);
}

void ZSharp::FZConjugateRegistry::HandleGarbageCollectComplete()
{
	IZMasterAssemblyLoadContext* alc = IZSharpCLR::Get().GetMasterALC();
    if (!alc)
    {
    	return;
    }
    	
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
		alc->ReleaseConjugate(unmanaged);
	}
}

void ZSharp::FZConjugateRegistry::HandleMasterALCLoaded(IZMasterAssemblyLoadContext* alc)
{
	check(ObjectRegistry.IsEmpty());
	check(StringRegistry.IsEmpty());

	FCoreUObjectDelegates::GarbageCollectComplete.AddRaw(this, &FZConjugateRegistry::HandleGarbageCollectComplete);
}

void ZSharp::FZConjugateRegistry::HandleMasterALCUnloaded()
{
	ObjectRegistry.Reset();
	StringRegistry.Reset();

	FCoreUObjectDelegates::GarbageCollectComplete.RemoveAll(this);
}


