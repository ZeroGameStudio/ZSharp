// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Conjugate/IZUObjectConjugateController.h"

#include "ZUObjectConjugateController_Actor.generated.h"

class UZUObjectConjugateController_Actor_Proxy;

namespace ZSharp
{
	class FZUObjectConjugateController_Actor : public IZUObjectConjugateController
	{

		friend UZUObjectConjugateController_Actor_Proxy;

	public:
		FZUObjectConjugateController_Actor();
		virtual ~FZUObjectConjugateController_Actor() override;

	private:
		virtual bool CanBuildConjugate(UObject* unmanaged) const override;
		virtual void NotifyConjugated(UObject* unmanaged) override;
		virtual void NotifyConjugateReleased(UObject* unmanaged) override;
		virtual void SetLifecycleExpiredCallback(const TFunction<void(UObject*)>& callback) override;

	private:
		UZUObjectConjugateController_Actor_Proxy* GetProxy() const;
		void HandleActorDestroyed(AActor* actor);

	private:
		mutable TStrongObjectPtr<UZUObjectConjugateController_Actor_Proxy> Proxy;
		TSet<TObjectKey<AActor>> Actors;
		TFunction<void(UObject*)> OnExpired;
		
	};
}

UCLASS(MinimalAPI, meta = (ZSharpNoExport))
class UZUObjectConjugateController_Actor_Proxy : public UObject
{
	GENERATED_BODY()
	
	friend ZSharp::FZUObjectConjugateController_Actor;

private:
	UFUNCTION()
	void HandleActorDestroyed(AActor* actor);

	ZSharp::FZUObjectConjugateController_Actor* Owner;
	
};


