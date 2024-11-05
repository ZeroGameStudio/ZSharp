// Copyright Zero Games. All Rights Reserved.


#include "Conjugate/ZRegularConjugateRegistries.h"

#include "Conjugate/ZDeclareConjugateRegistryMacros.h"

namespace ZSharp
{
#define IMPLEMENT_REGISTRY(Type) ZSHARP_DECLARE_CONJUGATE_REGISTRY(FZConjugateRegistry_##Type)

	IMPLEMENT_REGISTRY(String)
	IMPLEMENT_REGISTRY(Name)
	IMPLEMENT_REGISTRY(Text)

	IMPLEMENT_REGISTRY(FieldPath)

	#undef IMPLEMENT_REGISTRY
}


