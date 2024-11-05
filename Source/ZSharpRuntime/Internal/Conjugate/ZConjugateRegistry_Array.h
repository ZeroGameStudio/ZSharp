// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZConjugateRegistryBase.h"
#include "Interop/ZRuntimeTypeHandle.h"
#include "Conjugate/ZConjugateHandle.h"

namespace ZSharp
{
	struct FZSelfDescriptiveScriptArray;
	
	class ZSHARPRUNTIME_API FZConjugateRegistry_Array : public TZConjugateRegistryBase<FZConjugateRegistry_Array, FZSelfDescriptiveScriptArray>
	{
		ZSHARP_CONJUGATE_REGISTRY_GENERATED_BODY(FZConjugateRegistry_Array)

	public:
		// FZConjugateHandle Conjugate(const FProperty* elementProperty) { return Conjugate(elementProperty, ); }
		FZConjugateHandle Conjugate(const FProperty* elementProperty, TFunctionRef<void(const FZSelfDescriptiveScriptArray&)> initialize = [](const FZSelfDescriptiveScriptArray&){});
		FZConjugateHandle Conjugate(const FProperty* elementProperty, FScriptArray* unmanaged);

	private:
		static FZSelfDescriptiveScriptArray* BuildConjugateWrapper(void* userdata);

	private:
		FZRuntimeTypeHandle GetManagedType(const FProperty* elementProperty) const;

	};
}


