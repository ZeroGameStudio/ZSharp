// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZCall/IZConjugateRegistry.h"
#include "ZCall/ZConjugateHandle.h"

namespace ZSharp
{
	class IZMasterAssemblyLoadContext;
	
	class ZSHARPRUNTIME_API FZConjugateRegistry_String : public IZConjugateRegistry
	{

		using ThisClass = FZConjugateRegistry_String;

	public:
		static constexpr uint16 RegistryId = 2;

	public:
		explicit FZConjugateRegistry_String(IZMasterAssemblyLoadContext& alc);

	public:
		FString* Conjugate(FZConjugateHandle handle) const;
		FZConjugateHandle Conjugate(FString* unmanaged, bool bOwning);
		FZConjugateHandle BuildConjugate(FString* unmanaged, bool bOwning);
	
	private:
		virtual void Release() override;
		virtual void* BuildConjugate() override;
		virtual void ReleaseConjugate(void* unmanaged) override;
		virtual void PushRedFrame() override;
		virtual void PopRedFrame() override;

	private:
		void InternalReleaseConjugate(void* unmanaged);

	private:
		IZMasterAssemblyLoadContext& Alc;
		
		struct FZRec
		{
			FString* TypedUnmanaged;
			bool bOwning;
			bool bBlack;
		};
		TMap<void*, FZRec> ConjugateMap;
		struct FZRedFrame
		{
			TArray<void*> CapturedConjugates;
		};
		TArray<FZRedFrame> RedStack;
		
	};
}


