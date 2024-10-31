// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZConjugateRegistryBase.h"
#include "Reflection/Wrapper/ZSelfDescriptiveMulticastSparseScriptDelegate.h"
#include "Interop/ZRuntimeTypeHandle.h"
#include "Trait/ZConjugateRegistryId.h"
#include "Conjugate/ZConjugateHandle.h"

namespace ZSharp
{
	class IZMasterAssemblyLoadContext;
	
	class ZSHARPRUNTIME_API FZConjugateRegistry_MulticastSparseDelegate : public FZConjugateRegistryBase, public FNoncopyable
	{

		using Super = FZConjugateRegistryBase;
		using ThisClass = FZConjugateRegistry_MulticastSparseDelegate;

	public:
		static constexpr uint16 RegistryId = TZConjugateRegistryId_V<FZSelfDescriptiveMulticastSparseScriptDelegate>;

	public:
		explicit FZConjugateRegistry_MulticastSparseDelegate(IZMasterAssemblyLoadContext& alc) : Super(alc){}

	public:
		FZConjugateHandle Conjugate(const USparseDelegateFunction* signature, const FSparseDelegate* unmanaged);
		FZSelfDescriptiveMulticastSparseScriptDelegate* Conjugate(FZConjugateHandle handle) const;

	private:
		virtual void* BuildConjugate(void* userdata) override;
		virtual void ReleaseConjugate(void* unmanaged) override;
		virtual void GetAllConjugates(TArray<void*>& outConjugates) const override;

	private:
		FZRuntimeTypeHandle GetManagedType(const USparseDelegateFunction* signature) const;
		
	private:
		TMap<void*, TUniquePtr<FZSelfDescriptiveMulticastSparseScriptDelegate>> ConjugateMap;
	
	};
}


