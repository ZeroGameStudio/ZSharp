// Copyright Zero Games. All Rights Reserved.

#include "ZUObjectConjugateController_GC.h"

ZSharp::FZUObjectConjugateController_GC::FZUObjectConjugateController_GC()
{
	GCDelegate = FCoreUObjectDelegates::GarbageCollectComplete.AddRaw(this, &ThisClass::HandleGarbageCollectComplete);
}

ZSharp::FZUObjectConjugateController_GC::~FZUObjectConjugateController_GC()
{
	FCoreUObjectDelegates::GarbageCollectComplete.Remove(GCDelegate);
}

bool ZSharp::FZUObjectConjugateController_GC::CanBuildConjugate(UObject* unmanaged) const
{
	return !unmanaged->IsUnreachable();
}

void ZSharp::FZUObjectConjugateController_GC::NotifyConjugated(UObject* unmanaged)
{
	ConjugateMap.Emplace(unmanaged, unmanaged);
}

void ZSharp::FZUObjectConjugateController_GC::NotifyConjugateReleased(UObject* unmanaged)
{
	ConjugateMap.Remove(unmanaged);
}

void ZSharp::FZUObjectConjugateController_GC::SetLifecycleExpiredCallback(const TFunction<void(UObject*)>& callback)
{
	OnExpired = callback;
}

void ZSharp::FZUObjectConjugateController_GC::HandleGarbageCollectComplete()
{
	TArray<UObject*> pendingRemoves;
	for (const auto& pair : ConjugateMap)
	{
		if (!pair.Value.ResolveObjectPtrEvenIfGarbage())
		{
			pendingRemoves.Emplace(static_cast<UObject*>(pair.Key));
		}
	}
	
	for (const auto& key : pendingRemoves)
	{
		OnExpired(key);
		ConjugateMap.Remove(key);
	}
}


