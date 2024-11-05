// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZStrangeConjugateRegistryBase.h"
#include "Interop/ZRuntimeTypeHandle.h"
#include "Trait/ZConjugateRegistryId.h"

namespace ZSharp
{
	struct FZSelfDescriptiveOptional;
	
	class ZSHARPRUNTIME_API FZConjugateRegistry_Optional : public TZStrangeConjugateRegistryBase<FZConjugateRegistry_Optional, FZSelfDescriptiveOptional>
	{

		friend Super;
		friend TZStrangeConjugateRegistryBase;

	public:
		FZConjugateRegistry_Optional(IZMasterAssemblyLoadContext& alc) : TZStrangeConjugateRegistryBase(alc){}

	private:
		static FZSelfDescriptiveOptional* BuildConjugateWrapper(void* userdata);
		static void ValidateConjugateWrapper(const FProperty* elementProperty, const FZSelfDescriptiveOptional* wrapper);

	private:
		FZRuntimeTypeHandle GetManagedType(const FProperty* elementProperty) const;
	
	};
}


