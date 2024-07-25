// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	class ZSHARPRUNTIME_API IZUObjectConjugateController
	{
		friend class FZConjugateRegistry_UObject;
	public:
		virtual ~IZUObjectConjugateController(){}
	private:
		virtual bool CanBuildConjugate(UObject* unmanaged) const = 0;
		virtual void NotifyConjugated(UObject* unmanaged) = 0;
		virtual void NotifyConjugateReleased(UObject* unmanaged) = 0;
		virtual void SetLifecycleExpiredCallback(const TFunction<void(UObject*)>& callback) = 0;
	};
}


