// Copyright Zero Games. All Rights Reserved.


#include "ZUObjectConjugateController_Actor.h"

ZSharp::FZUObjectConjugateController_Actor::FZUObjectConjugateController_Actor()
{
	Proxy = TStrongObjectPtr { NewObject<UZUObjectConjugateController_Actor_Proxy>() };
	Proxy->Owner = this;
}

ZSharp::FZUObjectConjugateController_Actor::~FZUObjectConjugateController_Actor()
{
	for (const auto& key : Actors)
	{
		if (AActor* actor = key.ResolveObjectPtrEvenIfGarbage())
		{
			actor->OnDestroyed.RemoveAll(Proxy.Get());
		}
	}
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
		actor->OnDestroyed.AddDynamic(Proxy.Get(), &UZUObjectConjugateController_Actor_Proxy::HandleActorDestroyed);
	}
}

void ZSharp::FZUObjectConjugateController_Actor::NotifyConjugateReleased(UObject* unmanaged)
{
	if (AActor* actor = Cast<AActor>(unmanaged))
	{
		Actors.Remove(actor);
	}
}

void ZSharp::FZUObjectConjugateController_Actor::SetLifecycleExpiredCallback(const TFunction<void(UObject*)>& callback)
{
	OnExpired = callback;
}

void ZSharp::FZUObjectConjugateController_Actor::HandleActorDestroyed(AActor* actor)
{
	OnExpired(actor);
	actor->ForEachComponent(false, [this](UActorComponent* component)
	{
		OnExpired(component);
	});
	Actors.Remove(actor);
}

void UZUObjectConjugateController_Actor_Proxy::HandleActorDestroyed(AActor* actor)
{
	Owner->HandleActorDestroyed(actor);
}


