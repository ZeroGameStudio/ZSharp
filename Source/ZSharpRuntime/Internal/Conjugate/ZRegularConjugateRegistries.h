// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZRegularConjugateRegistryBase.h"

namespace ZSharp
{
#define DECLARE_REGISTRY(Type, IsGCObject) \
class ZSHARPRUNTIME_API FZConjugateRegistry_##Type : public TZRegularConjugateRegistryBase<FZConjugateRegistry_##Type, F##Type, IsGCObject> \
{ \
	friend Super; \
public: \
	FZConjugateRegistry_##Type(IZMasterAssemblyLoadContext& alc) : TZRegularConjugateRegistryBase(alc){} \
private: \
	static ConjugateWrapperType* BuildConjugateWrapper(void* userdata) { return new ConjugateWrapperType { new ConjugateType{}, true }; } \
};

	DECLARE_REGISTRY(String, false)
	DECLARE_REGISTRY(Utf8String, false)
	DECLARE_REGISTRY(AnsiString, false)
	DECLARE_REGISTRY(Name, false)
	DECLARE_REGISTRY(Text, false)

	DECLARE_REGISTRY(FieldPath, true)

#undef DECLARE_REGISTRY
}


