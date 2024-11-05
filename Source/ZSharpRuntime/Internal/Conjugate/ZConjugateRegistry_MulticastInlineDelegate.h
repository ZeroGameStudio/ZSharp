// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZStrangeConjugateRegistryBase.h"
#include "Interop/ZRuntimeTypeHandle.h"

namespace ZSharp
{
	struct FZSelfDescriptiveMulticastInlineScriptDelegate;
	
	class ZSHARPRUNTIME_API FZConjugateRegistry_MulticastInlineDelegate : public TZStrangeConjugateRegistryBase<FZConjugateRegistry_MulticastInlineDelegate, FZSelfDescriptiveMulticastInlineScriptDelegate>
	{

		friend Super;
		friend TZStrangeConjugateRegistryBase;

	public:
		FZConjugateRegistry_MulticastInlineDelegate(IZMasterAssemblyLoadContext& alc) : TZStrangeConjugateRegistryBase(alc){}

	private:
		static FZSelfDescriptiveMulticastInlineScriptDelegate* BuildConjugateWrapper(void* userdata);
		static void ValidateConjugateWrapper(const UDelegateFunction* descriptor, const FZSelfDescriptiveMulticastInlineScriptDelegate* wrapper);

	private:
		FZRuntimeTypeHandle GetManagedType(const UDelegateFunction* descriptor) const;
	
	};
}


