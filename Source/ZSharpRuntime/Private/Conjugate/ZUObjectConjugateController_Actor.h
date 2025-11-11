// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Conjugate/IZUObjectConjugateController.h"

namespace ZSharp
{
	class FZUObjectConjugateController_Actor : public IZUObjectConjugateController
	{

		using ThisClass = FZUObjectConjugateController_Actor;

	public:
		FZUObjectConjugateController_Actor();
		virtual ~FZUObjectConjugateController_Actor() override;

	private:
		virtual bool CanBuildConjugate(UObject* unmanaged) const override;
		virtual void NotifyConjugated(UObject* unmanaged) override;
		virtual void NotifyConjugateReleased(UObject* unmanaged) override;
		virtual void SetLifetimeExpiredCallback(const TFunction<void(UObject*)>& callback) override;
		
	private:
		void HandleWorldCreated(UWorld* world);
		void HandleActorDestroyed(AActor* actor);

	private:
		TMap<TObjectKey<UWorld>, FDelegateHandle> WorldRemoveActorDelegateMap;
		TSet<TObjectKey<AActor>> Actors;
		TFunction<void(UObject*)> OnExpired;
		
	};
}


