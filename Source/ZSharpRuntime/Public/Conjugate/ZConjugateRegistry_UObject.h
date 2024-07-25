// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/ZRuntimeTypeHandle.h"
#include "Trait/ZConjugateRegistryId.h"
#include "Conjugate/ZConjugateHandle.h"
#include "Conjugate/ZConjugateRegistryBase.h"
#include "Conjugate/IZUObjectConjugateController.h"

namespace ZSharp
{
	class ZSHARPRUNTIME_API FZConjugateRegistry_UObject : public FZConjugateRegistryBase, public FNoncopyable
	{

		using Super = FZConjugateRegistryBase;
		using ThisClass = FZConjugateRegistry_UObject;

	public:
		static constexpr uint16 RegistryId = GUObjectConjugateRegistryId;

	public:
		explicit FZConjugateRegistry_UObject(IZMasterAssemblyLoadContext& alc);

	public:
		UObject* Conjugate(FZConjugateHandle handle) const;
		FZConjugateHandle Conjugate(const UObjectBase* unmanaged);

	public:
		void RegisterController(IZUObjectConjugateController* controller);

	private:
		virtual void* BuildConjugate(void* userdata) override;
		virtual void ReleaseConjugate(void* unmanaged) override;
		virtual void PushRedFrame() override;
		virtual void PopRedFrame() override;
		virtual void GetAllConjugates(TArray<void*>& outConjugates) const override;

	private:
		bool CanBuildConjugate(UObject* unmanaged) const;
		void NotifyConjugated(UObject* unmanaged);
		void NotifyLifecycleExpired(UObject* unmanaged);

	private:
		FZRuntimeTypeHandle GetManagedType(const UObject* unmanaged) const;

	private:
		TMap<void*, FObjectKey> ConjugateMap;

		TArray<TUniquePtr<IZUObjectConjugateController>> Controllers;
		
	};
}


