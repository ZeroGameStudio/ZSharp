// Copyright Zero Games. All Rights Reserved.


#include "Conjugate/ZConjugateRegistry_UObject.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "Conjugate/ZDeclareConjugateRegistry.h"
#include "Reflection/ZReflectionHelper.h"

namespace ZSharp::ZConjugateRegistry_UObject_Private
{
	static TZDeclareConjugateRegistry<FZConjugateRegistry_UObject> GDeclare;
}

ZSharp::FZConjugateRegistry_UObject::FZConjugateRegistry_UObject(IZMasterAssemblyLoadContext& alc)
	: Super(alc)
{
	GCDelegate = FCoreUObjectDelegates::GarbageCollectComplete.AddRaw(this, &ThisClass::HandleGarbageCollectComplete);
}

ZSharp::FZConjugateRegistry_UObject::~FZConjugateRegistry_UObject()
{
	FCoreUObjectDelegates::GarbageCollectComplete.Remove(GCDelegate);
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

	const FZRuntimeTypeHandle type = GetManagedType(unmanagedObject);
	if (Alc.BuildConjugate(unmanagedObject, type))
	{
		ConjugateMap.Emplace(unmanagedObject, unmanagedObject);
	
		return { unmanagedObject };
	}

	return {};
}

void* ZSharp::FZConjugateRegistry_UObject::BuildConjugate(void* userdata)
{
	// There is no black UObject conjugate.
	checkNoEntry();
	return nullptr;
}

void ZSharp::FZConjugateRegistry_UObject::ReleaseConjugate(void* unmanaged)
{
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

ZSharp::FZRuntimeTypeHandle ZSharp::FZConjugateRegistry_UObject::GetManagedType(const UObject* unmanaged) const
{
	FZRuntimeTypeLocatorWrapper locator;
	if (!FZReflectionHelper::GetUFieldRuntimeTypeLocator(unmanaged->GetClass(), locator))
	{
		return {};
	}
	
	return Alc.GetType(locator);
}

void ZSharp::FZConjugateRegistry_UObject::HandleGarbageCollectComplete()
{
	TArray<void*> pendingRemoves;
	for (const auto& pair : ConjugateMap)
	{
		if (!pair.Value.ResolveObjectPtrEvenIfGarbage())
		{
			pendingRemoves.Emplace(pair.Key);
		}
	}
	
	for (const auto& key : pendingRemoves)
	{
		ReleaseConjugate(key);
	}
}


