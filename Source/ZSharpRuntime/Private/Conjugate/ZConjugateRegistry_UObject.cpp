// Copyright Zero Games. All Rights Reserved.


#include "Conjugate/ZConjugateRegistry_UObject.h"

#include "ZUObjectConjugateController_Actor.h"
#include "ZUObjectConjugateController_GC.h"
#include "ALC/IZMasterAssemblyLoadContext.h"
#include "ALC/ZRuntimeTypeUri.h"
#include "Conjugate/ZDeclareConjugateRegistry.h"
#include "Reflection/ZReflectionHelper.h"

namespace ZSharp::ZConjugateRegistry_UObject_Private
{
	static TZDeclareConjugateRegistry<FZConjugateRegistry_UObject> GDeclare;
}

ZSharp::FZConjugateRegistry_UObject::FZConjugateRegistry_UObject(IZMasterAssemblyLoadContext& alc)
	: Super(alc)
{
	RegisterController(new FZUObjectConjugateController_GC);
	RegisterController(new FZUObjectConjugateController_Actor);
}

UObject* ZSharp::FZConjugateRegistry_UObject::Conjugate(FZConjugateHandle handle) const
{
	void* unmanaged = handle.Handle;
	const FObjectKey* rec = ConjugateMap.Find(unmanaged);
	return rec ? rec->ResolveObjectPtrEvenIfGarbage() : nullptr;
}

ZSharp::FZConjugateHandle ZSharp::FZConjugateRegistry_UObject::Conjugate(const UObjectBase* unmanaged)
{
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

void* ZSharp::FZConjugateRegistry_UObject::BuildConjugate(void* userdata)
{
	// There is no black UObject conjugate.
	checkNoEntry();
	return nullptr;
}

void ZSharp::FZConjugateRegistry_UObject::ReleaseConjugate(void* unmanaged)
{
	if (!ConjugateMap.Contains(unmanaged))
	{
		return;
	}
	
	Alc.ReleaseConjugate(unmanaged);
	ConjugateMap.Remove(unmanaged);
}

void ZSharp::FZConjugateRegistry_UObject::PushRedFrame()
{
	// UObject conjugate is released by UEGC, not red frame.
}

void ZSharp::FZConjugateRegistry_UObject::PopRedFrame()
{
	// UObject conjugate is released by UEGC, not red frame.
}

void ZSharp::FZConjugateRegistry_UObject::GetAllConjugates(TArray<void*>& outConjugates) const
{
	for (const auto& pair : ConjugateMap)
	{
		outConjugates.Emplace(pair.Key);
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

void ZSharp::FZConjugateRegistry_UObject::NotifyConjugated(UObject* unmanaged)
{
	for (const auto& controller : Controllers)
	{
		controller->NotifyConjugated(unmanaged);
	}
}

void ZSharp::FZConjugateRegistry_UObject::NotifyLifecycleExpired(UObject* unmanaged)
{
	ReleaseConjugate(unmanaged);
}

ZSharp::FZRuntimeTypeHandle ZSharp::FZConjugateRegistry_UObject::GetManagedType(const UObject* unmanaged) const
{
	FZRuntimeTypeUri uri;
	if (!FZReflectionHelper::GetUFieldRuntimeTypeLocator(unmanaged->GetClass(), uri))
	{
		return {};
	}
	
	return Alc.GetType(uri);
}


