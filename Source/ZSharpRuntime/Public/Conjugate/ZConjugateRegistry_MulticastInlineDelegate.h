// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZConjugateRegistryBase.h"
#include "Reflection/Wrapper/ZSelfDescriptiveMulticastInlineScriptDelegate.h"
#include "Interop/ZRuntimeTypeHandle.h"
#include "Trait/ZConjugateRegistryId.h"
#include "Conjugate/ZConjugateHandle.h"

namespace ZSharp
{
	class IZMasterAssemblyLoadContext;
	
	class ZSHARPRUNTIME_API FZConjugateRegistry_MulticastInlineDelegate : public FZConjugateRegistryBase, public FNoncopyable
	{

		using Super = FZConjugateRegistryBase;
		using ThisClass = FZConjugateRegistry_MulticastInlineDelegate;

	public:
		static constexpr uint16 RegistryId = TZConjugateRegistryId_V<FZSelfDescriptiveMulticastInlineScriptDelegate>;

	private:
		struct FZConjugateRec
		{
			TUniquePtr<FZSelfDescriptiveMulticastInlineScriptDelegate> Delegate;
			bool bBlack;
		};

	public:
		explicit FZConjugateRegistry_MulticastInlineDelegate(IZMasterAssemblyLoadContext& alc) : Super(alc){}

	public:
		FZConjugateHandle Conjugate(const UFunction* signature) { return Conjugate(signature, [](const FZSelfDescriptiveMulticastInlineScriptDelegate&){}); }
		FZConjugateHandle Conjugate(const UFunction* signature, TFunctionRef<void(const FZSelfDescriptiveMulticastInlineScriptDelegate&)> initialize);
		FZConjugateHandle Conjugate(const UFunction* signature, const FMulticastScriptDelegate* unmanaged);
		FZSelfDescriptiveMulticastInlineScriptDelegate* Conjugate(FZConjugateHandle handle) const;

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


