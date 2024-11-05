// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZStrangeConjugateRegistryBase.h"
#include "Interop/ZRuntimeTypeHandle.h"

namespace ZSharp
{
	struct FZSelfDescriptiveMulticastSparseScriptDelegate;
	
	class ZSHARPRUNTIME_API FZConjugateRegistry_MulticastSparseDelegate : public TZStrangeConjugateRegistryBase<FZConjugateRegistry_MulticastSparseDelegate, FZSelfDescriptiveMulticastSparseScriptDelegate>
	{

		friend Super;
		friend TZStrangeConjugateRegistryBase;

	public:
		FZConjugateRegistry_MulticastSparseDelegate(IZMasterAssemblyLoadContext& alc) : TZStrangeConjugateRegistryBase(alc){}

	private:
		static FZSelfDescriptiveMulticastSparseScriptDelegate* BuildConjugateWrapper(void* userdata);
		static void ValidateConjugateWrapper(const USparseDelegateFunction* descriptor, const FZSelfDescriptiveMulticastSparseScriptDelegate* wrapper);

	private:
		FZRuntimeTypeHandle GetManagedType(const USparseDelegateFunction* descriptor) const;
	
	};
}


