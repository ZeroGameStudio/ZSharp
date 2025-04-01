// Copyright Zero Games. All Rights Reserved.

#include "ZUObjectConjugateController_Actor.h"

ZSharp::FZUObjectConjugateController_Actor::FZUObjectConjugateController_Actor()
{
	FWorldDelegates::OnPostWorldCreation.AddRaw(this, &ThisClass::HandleWorldCreated);
}

ZSharp::FZUObjectConjugateController_Actor::~FZUObjectConjugateController_Actor()
{
	for (const auto& [key, delegate] : WorldRemoveActorDelegateMap)
	{
		if (UWorld* world = key.ResolveObjectPtrEvenIfGarbage())
		{
			world->RemoveOnActorRemovedFromWorldHandler(delegate);
		}
	}

	FWorldDelegates::OnPostWorldCreation.RemoveAll(this);
}

bool ZSharp::FZUObjectConjugateController_Actor::CanBuildConjugate(UObject* unmanaged) const
{
	if (const AActor* actor = Cast<AActor>(unmanaged))
	{
		return !actor->IsPendingKillPending();
	}
	else if (const UActorComponent* component = Cast<UActorComponent>(unmanaged))
	{
		return ::IsValid(component) && !component->IsBeingDestroyed();
	}

	return true;
}

void ZSharp::FZUObjectConjugateController_Actor::NotifyConjugated(UObject* unmanaged)
{
	if (AActor* actor = Cast<AActor>(unmanaged))
	{
		Actors.Emplace(actor);
	}
}

void ZSharp::FZUObjectConjugateController_Actor::NotifyConjugateReleased(UObject* unmanaged)
{
	// Also try remove because it may be released by another controller.
	if (AActor* actor = Cast<AActor>(unmanaged))
	{
		Actors.Remove(actor);
	}
}

void ZSharp::FZUObjectConjugateController_Actor::SetLifecycleExpiredCallback(const TFunction<void(UObject*)>& callback)
{
	OnExpired = callback;
}

void ZSharp::FZUObjectConjugateController_Actor::HandleWorldCreated(UWorld* world)
{
	if (world->IsTemplate())
	{
		return;
	}

	// NOTE: This is called in UWorld constructor, don't do too much.

	// It is highly likely some world is released when a new world comes up.
	TMap oldWorldRemoveActorDelegateMap { MoveTemp(WorldRemoveActorDelegateMap) };
	WorldRemoveActorDelegateMap.Reset();
	for (const auto& [key, delegate] : oldWorldRemoveActorDelegateMap)
	{
		if (key.ResolveObjectPtrEvenIfGarbage())
		{
			WorldRemoveActorDelegateMap.Emplace(key, delegate);
		}
	}
	
	FDelegateHandle delegate = world->AddOnActorRemovedFromWorldHandler(FOnActorRemovedFromWorld::FDelegate::CreateRaw(this, &ThisClass::HandleActorDestroyed));
	WorldRemoveActorDelegateMap.Emplace(world, delegate);
}

void ZSharp::FZUObjectConjugateController_Actor::HandleActorDestroyed(AActor* actor)
{
	if (!Actors.Remove(actor))
	{
		return;
	}
	
	OnExpired(actor);
	actor->ForEachComponent(false, [this](UActorComponent* component)
	{
		OnExpired(component);
	});
}


