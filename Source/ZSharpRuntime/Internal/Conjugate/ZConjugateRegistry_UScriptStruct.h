// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZStrangeConjugateRegistryBase.h"
#include "Interop/ZRuntimeTypeHandle.h"

namespace ZSharp
{
	struct FZSelfDescriptiveScriptStruct;
	
	class ZSHARPRUNTIME_API FZConjugateRegistry_UScriptStruct : public TZStrangeConjugateRegistryBase<FZConjugateRegistry_UScriptStruct, FZSelfDescriptiveScriptStruct>
	{

		friend Super;
		friend TZStrangeConjugateRegistryBase;

	public:
		FZConjugateRegistry_UScriptStruct(IZMasterAssemblyLoadContext& alc) : TZStrangeConjugateRegistryBase(alc){}

	private:
		static FZSelfDescriptiveScriptStruct* BuildConjugateWrapper(void* userdata);
		static void ValidateConjugateWrapper(const UScriptStruct* descriptor, const FZSelfDescriptiveScriptStruct* wrapper);

	private:
		FZRuntimeTypeHandle GetManagedType(const UScriptStruct* descriptor) const;
	
	};
}

