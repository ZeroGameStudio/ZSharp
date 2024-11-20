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
	FZConjugateRegistry_##Type(IZMasterAssemblyLoadContext& alc) : TZRegularConjugateRegistryBase(alc){} \
private: \
	static ConjugateWrapperType* BuildConjugateWrapper(void* userdata) { return new ConjugateWrapperType { new ConjugateType{}, true }; } \
};

	DECLARE_REGISTRY(String)
	DECLARE_REGISTRY(Utf8String)
	DECLARE_REGISTRY(AnsiString)
	DECLARE_REGISTRY(Name)
	DECLARE_REGISTRY(Text)

	DECLARE_REGISTRY(FieldPath)

#undef DECLARE_REGISTRY
}


