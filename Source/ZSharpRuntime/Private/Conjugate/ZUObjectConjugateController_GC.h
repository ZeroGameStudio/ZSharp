// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Conjugate/IZUObjectConjugateController.h"

namespace ZSharp
{
	class FZUObjectConjugateController_GC : public IZUObjectConjugateController
	{

		using ThisClass = FZUObjectConjugateController_GC;

	public:
		FZUObjectConjugateController_GC();
		virtual ~FZUObjectConjugateController_GC() override;

	private:
		virtual bool CanBuildConjugate(UObject* unmanaged) const override;
		virtual void NotifyConjugated(UObject* unmanaged) override;
		virtual void NotifyConjugateReleased(UObject* unmanaged) override;
		virtual void SetLifecycleExpiredCallback(const TFunction<void(UObject*)>& callback) override;

	private:
		void HandleGarbageCollectComplete();

	private:
		TMap<void*, FObjectKey> ConjugateMap;
		TFunction<void(UObject*)> OnExpired;

		FDelegateHandle GCDelegate;
		
	};
}


