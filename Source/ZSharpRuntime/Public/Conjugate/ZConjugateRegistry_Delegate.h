// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZConjugateRegistryBase.h"
#include "Reflection/Wrapper/ZSelfDescriptiveScriptDelegate.h"
#include "Interop/ZRuntimeTypeHandle.h"
#include "Trait/ZConjugateRegistryId.h"
#include "Conjugate/ZConjugateHandle.h"

namespace ZSharp
{
	class IZMasterAssemblyLoadContext;
	
	class ZSHARPRUNTIME_API FZConjugateRegistry_Delegate : public FZConjugateRegistryBase, public FNoncopyable
	{

		using Super = FZConjugateRegistryBase;
		using ThisClass = FZConjugateRegistry_Delegate;

	public:
		static constexpr uint16 RegistryId = TZConjugateRegistryId_V<FZSelfDescriptiveScriptDelegate>;

	private:
		struct FZConjugateRec
		{
			TUniquePtr<FZSelfDescriptiveScriptDelegate> Delegate;
			bool bBlack;
		};

	public:
		explicit FZConjugateRegistry_Delegate(IZMasterAssemblyLoadContext& alc) : Super(alc){}

	public:
		FZConjugateHandle Conjugate(const UFunction* signature) { return Conjugate(signature, [](const FZSelfDescriptiveScriptDelegate&){}); }
		FZConjugateHandle Conjugate(const UFunction* signature, TFunctionRef<void(const FZSelfDescriptiveScriptDelegate&)> initialize);
		FZConjugateHandle Conjugate(const UFunction* signature, const FScriptDelegate* unmanaged);
		FZSelfDescriptiveScriptDelegate* Conjugate(FZConjugateHandle handle) const;

	private:
		virtual void* BuildConjugate(void* userdata) override;
		virtual void ReleaseConjugate(void* unmanaged) override;
		virtual void GetAllConjugates(TArray<void*>& outConjugates) const override;

	private:
		FZRuntimeTypeHandle GetManagedType(const UFunction* signature) const;
		
	private:
		TMap<void*, FZConjugateRec> ConjugateMap;
	
	};
}


