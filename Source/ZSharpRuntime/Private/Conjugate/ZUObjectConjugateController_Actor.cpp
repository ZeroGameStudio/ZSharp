// Copyright Zero Games. All Rights Reserved.

#include "ZUObjectConjugateController_Actor.h"

ZSharp::FZUObjectConjugateController_Actor::FZUObjectConjugateController_Actor()
{

}

ZSharp::FZUObjectConjugateController_Actor::~FZUObjectConjugateController_Actor()
{
	if (!Proxy)
	{
		return;
	}

	UZUObjectConjugateController_Actor_Proxy* proxy = Proxy.Get();
	for (const auto& key : Actors)
	{
		if (AActor* actor = key.ResolveObjectPtrEvenIfGarbage())
		{
			actor->OnDestroyed.RemoveAll(proxy);
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
		// Reinstance copies this delegate to new instance before we remove.
		actor->OnDestroyed.AddUniqueDynamic(GetProxy(), &UZUObjectConjugateController_Actor_Proxy::HandleActorDestroyed);
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

UZUObjectConjugateController_Actor_Proxy* ZSharp::FZUObjectConjugateController_Actor::GetProxy() const
{
	UZUObjectConjugateController_Actor_Proxy* proxy = Proxy.Get();
	if (!proxy)
	{
		proxy = NewObject<UZUObjectConjugateController_Actor_Proxy>();
		proxy->Owner = const_cast<FZUObjectConjugateController_Actor*>(this);
		Proxy = TStrongObjectPtr { proxy };
	}

	return proxy;
}

void ZSharp::FZUObjectConjugateController_Actor::HandleActorDestroyed(AActor* actor)
{
	OnExpired(actor);
	actor->ForEachComponent(false, [this](UActorComponent* component)
	{
		OnExpired(component);
	});
	actor->OnDestroyed.RemoveAll(Proxy.Get());
	Actors.Remove(actor);
}

void UZUObjectConjugateController_Actor_Proxy::HandleActorDestroyed(AActor* actor)
{
	Owner->HandleActorDestroyed(actor);
}


