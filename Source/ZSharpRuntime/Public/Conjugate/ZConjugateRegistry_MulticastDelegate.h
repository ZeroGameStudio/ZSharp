// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZConjugateRegistryBase.h"
#include "Reflection/Wrapper/ZSelfDescriptiveMulticastScriptDelegate.h"
#include "Interop/ZRuntimeTypeHandle.h"
#include "Trait/ZConjugateRegistryId.h"
#include "Conjugate/ZConjugateHandle.h"

namespace ZSharp
{
	class IZMasterAssemblyLoadContext;
	
	class ZSHARPRUNTIME_API FZConjugateRegistry_MulticastDelegate : public FZConjugateRegistryBase, public FNoncopyable
	{

		using Super = FZConjugateRegistryBase;
		using ThisClass = FZConjugateRegistry_MulticastDelegate;

	public:
		static constexpr uint16 RegistryId = TZConjugateRegistryId_V<FZSelfDescriptiveMulticastScriptDelegate>;

	private:
		struct FZConjugateRec
		{
			TUniquePtr<FZSelfDescriptiveMulticastScriptDelegate> Delegate;
			bool bBlack;
		};

	public:
		explicit FZConjugateRegistry_MulticastDelegate(IZMasterAssemblyLoadContext& alc) : Super(alc){}

	public:
		FZConjugateHandle Conjugate(const UFunction* signature) { return Conjugate(signature, [](const FZSelfDescriptiveMulticastScriptDelegate&){}); }
		FZConjugateHandle Conjugate(const UFunction* signature, TFunctionRef<void(const FZSelfDescriptiveMulticastScriptDelegate&)> initialize);
		FZConjugateHandle Conjugate(const UFunction* signature, const FMulticastScriptDelegate* unmanaged);
		FZSelfDescriptiveMulticastScriptDelegate* Conjugate(FZConjugateHandle handle) const;

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


