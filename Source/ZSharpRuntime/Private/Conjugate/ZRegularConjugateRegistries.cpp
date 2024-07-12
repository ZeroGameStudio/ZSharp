// Copyright Zero Games. All Rights Reserved.


#include "Conjugate/ZRegularConjugateRegistries.h"

#include "Trait/ZExportedTypeName.h"
#include "Trait/ZExportedTypeModule.h"
#include "Conjugate/ZDeclareConjugateRegistry.h"

#define COMBINE_INNER(A, B) A##B
#define COMBINE(A, B) COMBINE_INNER(A, B)
#define IMPLEMENT_REGISTRY(Type) \
ZSharp::FZRuntimeTypeHandle ZSharp::FZConjugateRegistry_##Type::GetManagedType(const ConjugateType* unmanaged) const \
{ \
	FZRuntimeTypeLocatorWrapper locator; \
	locator.AssemblyName = ZSHARP_ENGINE_ASSEMBLY_NAME; \
	locator.TypeName = FString::Printf(TEXT("%s.%s.%s"), TEXT(ZSHARP_ENGINE_ASSEMBLY_NAME), *TZExportedTypeModule<F##Type>::Get(), *TZExportedTypeName<F##Type>::Get()); \
	return Alc.GetType(locator); \
} \
namespace ZSharp::ZRegularConjugateRegistries_Private \
{ \
	static TZDeclareConjugateRegistry<FZConjugateRegistry_##Type> COMBINE(GDeclare, __LINE__); \
}

IMPLEMENT_REGISTRY(String)
IMPLEMENT_REGISTRY(Name)
IMPLEMENT_REGISTRY(Text)

IMPLEMENT_REGISTRY(FieldPath)

#undef IMPLEMENT_REGISTRY
#undef COMBINE
#undef COMBINE_INNER


