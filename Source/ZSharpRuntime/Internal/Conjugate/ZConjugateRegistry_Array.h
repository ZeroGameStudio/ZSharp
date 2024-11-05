// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZStrangeConjugateRegistryBase.h"
#include "Interop/ZRuntimeTypeHandle.h"

namespace ZSharp
{
	struct FZSelfDescriptiveScriptArray;
	
	class ZSHARPRUNTIME_API FZConjugateRegistry_Array : public TZStrangeConjugateRegistryBase<FZConjugateRegistry_Array, FZSelfDescriptiveScriptArray>
	{

		friend Super;
		friend TZStrangeConjugateRegistryBase;

	public:
		FZConjugateRegistry_Array(IZMasterAssemblyLoadContext& alc) : TZStrangeConjugateRegistryBase(alc){}

	private:
		static FZSelfDescriptiveScriptArray* BuildConjugateWrapper(void* userdata);
		static void ValidateConjugateWrapper(const FProperty* elementProperty, const FZSelfDescriptiveScriptArray* wrapper);

	private:
		FZRuntimeTypeHandle GetManagedType(const FProperty* elementProperty) const;

	};
}


