// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZObjectWrapperConjugateRegistryBase.h"
#include "Reflection/Wrapper/ZSelfDescriptiveObjectWrappers.h"

namespace ZSharp
{
#define ZSHARP_STRINGFY_REFERENCER_NAME_INNER(Text) TEXT(#Text)
#define ZSHARP_STRINGFY_REFERENCER_NAME(Text) ZSHARP_STRINGFY_REFERENCER_NAME_INNER(Text)
#define ZSHARP_COMBINE_REFERENCER_NAME(A, B) A##B
	
#define DECLARE_REGISTRY(Type, IsGCObject) \
class ZSHARPRUNTIME_API FZConjugateRegistry_##Type : public TZObjectWrapperConjugateRegistryBase<FZConjugateRegistry_##Type, FZSelfDescriptive##Type, IsGCObject> \
{ \
	friend Super; \
public: \
	FZConjugateRegistry_##Type(IZMasterAssemblyLoadContext& alc) : TZObjectWrapperConjugateRegistryBase(alc){} \
private: \
	static ConjugateWrapperType* BuildConjugateWrapper(void* userdata) \
	{ \
		auto descriptor = static_cast<const UClass*>(userdata); \
		return new ConjugateWrapperType { descriptor }; \
	} \
private: \
	virtual FString GetReferencerName() const override { return ZSHARP_STRINGFY_REFERENCER_NAME(ZSHARP_COMBINE_REFERENCER_NAME(ZConjugateRegistry_, Type)); } \
};

	DECLARE_REGISTRY(SubclassOf, true)
	DECLARE_REGISTRY(SoftClassPtr, false)
	DECLARE_REGISTRY(SoftObjectPtr, false)
	DECLARE_REGISTRY(WeakObjectPtr, false)
	DECLARE_REGISTRY(LazyObjectPtr, false)
	DECLARE_REGISTRY(ScriptInterface, true)

#undef DECLARE_REGISTRY
#undef ZSHARP_COMBINE_REFERENCER_NAME
#undef ZSHARP_STRINGFY_REFERENCER_NAME
#undef ZSHARP_STRINGFY_REFERENCER_NAME_INNER
}


