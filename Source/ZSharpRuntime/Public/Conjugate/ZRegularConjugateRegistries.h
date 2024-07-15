// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZRegularConjugateRegistryBase.h"

namespace ZSharp
{
#define DECLARE_REGISTRY(Type) \
	class ZSHARPRUNTIME_API FZConjugateRegistry_##Type : public TZRegularConjugateRegistryBase<FZConjugateRegistry_##Type, F##Type> \
	{ \
		friend Super; \
	public: \
		explicit FZConjugateRegistry_##Type(IZMasterAssemblyLoadContext& alc) : Super(alc){} \
	private: \
		FZRuntimeTypeHandle GetManagedType(const ConjugateType* unmanaged) const; \
	};

	DECLARE_REGISTRY(String)
	DECLARE_REGISTRY(Name)
	DECLARE_REGISTRY(Text)

	DECLARE_REGISTRY(FieldPath)

#undef DECLARE_REGISTRY
}


