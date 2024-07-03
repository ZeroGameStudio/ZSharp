// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/ZRuntimeTypeHandle.h"
#include "Trait/ZConjugateRegistryId.h"
#include "ZCall/ZConjugateHandle.h"
#include "ZCall/Conjugate/ZConjugateRegistryBase.h"

namespace ZSharp
{
	class ZSHARPRUNTIME_API FZConjugateRegistry_UObject : public FZConjugateRegistryBase
	{

		using Super = FZConjugateRegistryBase;
		using ThisClass = FZConjugateRegistry_UObject;

	public:
		static constexpr uint16 RegistryId = GUObjectConjugateRegistryId;

	public:
		explicit FZConjugateRegistry_UObject(IZMasterAssemblyLoadContext& alc);
		virtual ~FZConjugateRegistry_UObject() override;

	public:
		UObject* Conjugate(FZConjugateHandle handle) const;
		FZConjugateHandle Conjugate(const UObjectBase* unmanaged);
	
	private:
		virtual void* BuildConjugate(void* userdata) override;
		virtual void ReleaseConjugate(void* unmanaged) override;
		virtual void PushRedFrame() override;
		virtual void PopRedFrame() override;
		virtual void GetAllConjugates(TArray<void*>& outConjugates) const override;

	private:
		FZRuntimeTypeHandle GetManagedType(const UObject* unmanaged) const;

	private:
		void HandleGarbageCollectComplete();

	private:
		TMap<void*, FObjectKey> ConjugateMap;
		
		FDelegateHandle GCDelegate;
		
	};
}


