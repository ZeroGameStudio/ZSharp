﻿// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZConjugateRegistryBase.h"

namespace ZSharp
{
#define DECLARE_REGISTRY(Type) \
	class ZSHARPRUNTIME_API FZConjugateRegistry_##Type : public TZConjugateRegistryBase<FZConjugateRegistry_##Type, F##Type> \
	{ \
		friend class Super; \
	public: \
		explicit FZConjugateRegistry_##Type(IZMasterAssemblyLoadContext& alc) : Super(alc){} \
	private: \
		FZRuntimeTypeHandle GetManagedType(const ConjugateType* unmanaged) const; \
	};

	DECLARE_REGISTRY(String)
	DECLARE_REGISTRY(Name)
	DECLARE_REGISTRY(Text)
	
	DECLARE_REGISTRY(WeakObjectPtr)
	DECLARE_REGISTRY(SoftObjectPtr)
	DECLARE_REGISTRY(LazyObjectPtr)
	DECLARE_REGISTRY(StrongObjectPtr)
	DECLARE_REGISTRY(SoftClassPtr)
	DECLARE_REGISTRY(ScriptInterface)

#undef DECLARE_REGISTRY
}

