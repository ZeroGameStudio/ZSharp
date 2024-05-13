// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZCall/IZConjugateRegistry.h"
#include "ZCall/ZConjugateHandle.h"

namespace ZSharp
{
	class IZMasterAssemblyLoadContext;
	
	class ZSHARPRUNTIME_API FZConjugateRegistry_UObject : public IZConjugateRegistry
	{

		using ThisClass = FZConjugateRegistry_UObject;

	public:
		static constexpr uint16 RegistryId = 1;

	public:
		explicit FZConjugateRegistry_UObject(IZMasterAssemblyLoadContext& alc);
		virtual ~FZConjugateRegistry_UObject() override;

	public:
		UObject* Conjugate(FZConjugateHandle handle) const;
		FZConjugateHandle Conjugate(UObject* unmanaged);
	
	private:
		virtual void Release() override;
		virtual void* BuildConjugate() override;
		virtual void ReleaseConjugate(void* unmanaged) override;
		virtual void PushRedFrame() override;
		virtual void PopRedFrame() override;

	private:
		void InternalReleaseConjugate(void* unmanaged);

	private:
		void HandleGarbageCollectComplete();

	private:
		IZMasterAssemblyLoadContext& Alc;
		
		struct FZRec
		{
			FObjectKey TypedUnmanaged;
		};
		TMap<void*, FZRec> ConjugateMap;
		struct FZRedFrame
		{
			TArray<void*> CapturedConjugates;
		};
		TArray<FZRedFrame> RedStack;
		
		FDelegateHandle GCDelegate;
		
	};
}


