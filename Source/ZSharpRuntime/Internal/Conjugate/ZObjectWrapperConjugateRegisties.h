// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZObjectWrapperConjugateRegistryBase.h"
#include "Reflection/Wrapper/ZSelfDescriptiveObjectWrappers.h"

namespace ZSharp
{
#define DECLARE_REGISTRY(Type) \
class ZSHARPRUNTIME_API FZConjugateRegistry_##Type : public TZObjectWrapperConjugateRegistryBase<FZConjugateRegistry_##Type, FZSelfDescriptive##Type> \
{ \
	friend Super; \
public: \
	FZConjugateRegistry_##Type(IZMasterAssemblyLoadContext& alc) : TZObjectWrapperConjugateRegistryBase(alc){} \
private: \
	static ConjugateWrapperType* BuildConjugateWrapper(void* userdata) \
	{ \
		const auto descriptor = static_cast<UClass*>(userdata); \
		return new ConjugateWrapperType { descriptor }; \
	} \
};

	DECLARE_REGISTRY(SubclassOf)
	DECLARE_REGISTRY(SoftClassPtr)
	DECLARE_REGISTRY(SoftObjectPtr)
	DECLARE_REGISTRY(WeakObjectPtr)
	DECLARE_REGISTRY(LazyObjectPtr)
	DECLARE_REGISTRY(ScriptInterface)
	DECLARE_REGISTRY(StrongObjectPtr)
	
#undef DECLARE_REGISTRY
}


