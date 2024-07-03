// Copyright Zero Games. All Rights Reserved.

#pragma once
#include "ZCall/IZConjugateRegistry.h"

namespace ZSharp
{
	class IZMasterAssemblyLoadContext;
	
	namespace ZConjugateRegistryBase_Private
	{
		struct FZRedFrame
		{
			TArray<void*> CapturedConjugates;
		};
	}
	
	class FZConjugateRegistryBase : public IZConjugateRegistry
	{

	public:
		explicit FZConjugateRegistryBase(IZMasterAssemblyLoadContext& alc)
			: Alc(alc){}

	public:
		virtual void Release() override;
		virtual void PushRedFrame() override;
		virtual void PopRedFrame() override;

	protected:
		virtual void GetAllConjugates(TArray<void*>& outConjugates) const = 0;

	protected:
		void CaptureConjugate(void* unmanaged);

	protected:
		IZMasterAssemblyLoadContext& Alc;
		TArray<ZConjugateRegistryBase_Private::FZRedFrame> RedStack;
		
	};
}


