// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZStrangeConjugateRegistryBase.h"
#include "Interop/ZRuntimeTypeHandle.h"

namespace ZSharp
{
	struct FZSelfDescriptiveScriptSet;

	class ZSHARPRUNTIME_API FZConjugateRegistry_Set : public TZStrangeConjugateRegistryBase<FZConjugateRegistry_Set, FZSelfDescriptiveScriptSet>
	{

		friend Super;
		friend TZStrangeConjugateRegistryBase;

	public:
		FZConjugateRegistry_Set(IZMasterAssemblyLoadContext& alc) : TZStrangeConjugateRegistryBase(alc){}

	private:
		static FZSelfDescriptiveScriptSet* BuildConjugateWrapper(void* userdata);
		static void ValidateConjugateWrapper(const FProperty* elementProperty, const FZSelfDescriptiveScriptSet* wrapper);

	private:
		FZRuntimeTypeHandle GetManagedType(const FProperty* elementProperty) const;
	
	};
}


