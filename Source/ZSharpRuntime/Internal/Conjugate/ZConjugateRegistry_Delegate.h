// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZStrangeConjugateRegistryBase.h"
#include "Interop/ZRuntimeTypeHandle.h"

namespace ZSharp
{
	struct FZSelfDescriptiveScriptDelegate;
	
	class ZSHARPRUNTIME_API FZConjugateRegistry_Delegate : public TZStrangeConjugateRegistryBase<FZConjugateRegistry_Delegate, FZSelfDescriptiveScriptDelegate>
	{

		friend Super;
		friend TZStrangeConjugateRegistryBase;

	public:
		FZConjugateRegistry_Delegate(IZMasterAssemblyLoadContext& alc) : TZStrangeConjugateRegistryBase(alc){}

	private:
		static FZSelfDescriptiveScriptDelegate* BuildConjugateWrapper(void* userdata);
		static void ValidateConjugateWrapper(const UDelegateFunction* descriptor, const FZSelfDescriptiveScriptDelegate* wrapper);

	private:
		FZRuntimeTypeHandle GetManagedType(const UDelegateFunction* descriptor) const;
	
	};
}


