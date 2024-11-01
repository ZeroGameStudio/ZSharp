// Copyright Zero Games. All Rights Reserved.


#include "Conjugate/ZRegularConjugateRegistries.h"

#include "ALC/ZRuntimeTypeUri.h"
#include "Trait/ZManagedTypeInfo.h"
#include "Conjugate/ZDeclareConjugateRegistryMacros.h"

#define COMBINE_INNER(A, B) A##B
#define COMBINE(A, B) COMBINE_INNER(A, B)
#define IMPLEMENT_REGISTRY(Type) \
ZSHARP_DECLARE_CONJUGATE_REGISTRY(FZConjugateRegistry_##Type) \
ZSharp::FZRuntimeTypeHandle ZSharp::FZConjugateRegistry_##Type::GetManagedType(const ConjugateType* unmanaged) const \
{ \
	FZRuntimeTypeUri uri; \
	uri.AssemblyName = TZManagedTypeInfo<F##Type>::GetAssemblyName(); \
	uri.TypeName = TZManagedTypeInfo<F##Type>::GetFullName(); \
	return Alc.GetType(uri); \
}

IMPLEMENT_REGISTRY(String)
IMPLEMENT_REGISTRY(Name)
IMPLEMENT_REGISTRY(Text)

IMPLEMENT_REGISTRY(FieldPath)

#undef IMPLEMENT_REGISTRY
#undef COMBINE
#undef COMBINE_INNER


