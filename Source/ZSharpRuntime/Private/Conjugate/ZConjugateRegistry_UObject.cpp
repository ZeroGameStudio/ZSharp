// Copyright Zero Games. All Rights Reserved.

#include "Conjugate/ZConjugateRegistry_UObject.h"

#include "ZUObjectConjugateController_Actor.h"
#include "ZUObjectConjugateController_GC.h"
#include "ALC/IZMasterAssemblyLoadContext.h"
#include "ALC/ZRuntimeTypeUri.h"
#include "Conjugate/ZDeclareConjugateRegistryMacros.h"
#include "Reflection/ZReflectionHelper.h"

// IMPORTANT: We need this to get value of ZSHARP_ENABLE_CONJUGATE_UNSAFE.
#include "ALC/ZRedFrameScope.h"
#include "Conjugate/ZConjugateRegistryBase.h"

ZSHARP_DECLARE_CONJUGATE_REGISTRY(FZConjugateRegistry_UObject)

ZSharp::FZConjugateRegistry_UObject::FZConjugateRegistry_UObject(IZMasterAssemblyLoadContext& alc)
	: Alc(alc)
{
	RegisterController(new FZUObjectConjugateController_GC);
	RegisterController(new FZUObjectConjugateController_Actor);
}

UObject* ZSharp::FZConjugateRegistry_UObject::ConjugateUnsafe(FZConjugateHandle handle) const
{
#if ZSHARP_ENABLE_CONJUGATE_UNSAFE
	return static_cast<UObject*>(handle.Handle);
#else
	return Conjugate(handle);
#endif
}

UObject* ZSharp::FZConjugateRegistry_UObject::Conjugate(FZConjugateHandle handle) const
{
	if (!handle)
	{
		return nullptr;
	}
	
	void* unmanaged = handle.Handle;
	const FObjectKey* rec = ConjugateMap.Find(unmanaged);
	return rec ? rec->ResolveObjectPtrEvenIfGarbage() : nullptr;
}

ZSharp::FZConjugateHandle ZSharp::FZConjugateRegistry_UObject::Conjugate(const UObjectBase* unmanaged)
{
	checkSlow(IsInGameThread());
	
	auto unmanagedObject = (UObject*)unmanaged;
	if (!unmanagedObject)
	{
		return {};
	}
	
	if (ConjugateMap.Find(unmanagedObject))
	{
		return { unmanagedObject };
	}

	if (!CanBuildConjugate(unmanagedObject))
	{
		return {};
	}

	// Emitted class may have user defined red constructor.
	FZRedFrameScope scope;

	const FZRuntimeTypeHandle type = GetManagedType(unmanagedObject);
	if (Alc.BuildConjugate(unmanagedObject, type))
	{
		ConjugateMap.Emplace(unmanagedObject, unmanagedObject);
		NotifyConjugated(unmanagedObject);
	
		return { unmanagedObject };
	}

	return {};
}

void ZSharp::FZConjugateRegistry_UObject::RegisterController(IZUObjectConjugateController* controller)
{
	Controllers.Emplace(controller);

	controller->SetLifecycleExpiredCallback([this](UObject* unmanaged){ NotifyLifecycleExpired(unmanaged); });
	for (const auto& pair : ConjugateMap)
	{
		if (UObject* unmanaged = pair.Value.ResolveObjectPtrEvenIfGarbage())
		{
			controller->NotifyConjugated(unmanaged);
		}
	}
}

bool ZSharp::FZConjugateRegistry_UObject::CanBuildConjugate(UObject* unmanaged) const
{
	for (const auto& controller : Controllers)
	{
		if (!controller->CanBuildConjugate(unmanaged))
		{
			return false;
		}
	}

	return true;
}

void ZSharp::FZConjugateRegistry_UObject::Release()
{
	TArray<void*> reds;
	for (const auto& pair : ConjugateMap)
	{
		reds.Emplace(pair.Key);
	}

	for (const auto& red : reds)
	{
		ReleaseConjugate_Red(red);
	}
}

void* ZSharp::FZConjugateRegistry_UObject::BuildConjugate(void* userdata)
{
	// There is no black UObject conjugate.
	checkNoEntry();
	return nullptr;
}

void ZSharp::FZConjugateRegistry_UObject::ReleaseConjugate(void* unmanaged)
{
	// There is no black UObject conjugate.
	checkNoEntry();
}

void ZSharp::FZConjugateRegistry_UObject::PushRedFrame()
{
	// UObject conjugate is released by UEGC, not red frame.
}

void ZSharp::FZConjugateRegistry_UObject::PopRedFrame()
{
	// UObject conjugate is released by UEGC, not red frame.
}

void ZSharp::FZConjugateRegistry_UObject::ReleaseConjugate_Red(void* unmanaged)
{
	if (ConjugateMap.Remove(unmanaged))
	{
		Alc.ReleaseConjugate(unmanaged);
		for (const auto& controller : Controllers)
		{
			controller->NotifyConjugateReleased(static_cast<UObject*>(unmanaged));
		}
	}
}

void ZSharp::FZConjugateRegistry_UObject::NotifyConjugated(UObject* unmanaged)
{
	for (const auto& controller : Controllers)
	{
		controller->NotifyConjugated(unmanaged);
	}
}

void ZSharp::FZConjugateRegistry_UObject::NotifyLifecycleExpired(UObject* unmanaged)
{
	ReleaseConjugate_Red(unmanaged);
}

ZSharp::FZRuntimeTypeHandle ZSharp::FZConjugateRegistry_UObject::GetManagedType(const UObject* unmanaged) const
{
	const UClass* cls = unmanaged->GetClass();
	if (const FZRuntimeTypeHandle* handle = ManagedTypeLookup.Find(cls))
	{
		return *handle;
	}

	FZRuntimeTypeHandle handle{};
	const UClass* currentClass = cls;
	while (currentClass)
	{
		if (currentClass->IsNative())
		{
			FZRuntimeTypeUri uri { FZReflectionHelper::GetFieldConjugateKey(currentClass) };
			if (FZRuntimeTypeHandle type = Alc.GetType(uri))
			{
				handle = type;
				break;
			}
		}

		currentClass = currentClass->GetSuperClass();
	}

	check(handle);
	ManagedTypeLookup.Emplace(cls, handle);
	return handle;
}


