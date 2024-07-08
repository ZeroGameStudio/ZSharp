// Copyright Zero Games. All Rights Reserved.


#include "ZCall/Conjugate/ZRegularConjugateRegistries.h"

#include "Trait/ZExportedTypeModule.h"
#include "ZCall/ZDeclareConjugateRegistry.h"
#include "ZCall/Export/ZExports.h"

#define COMBINE_INNER(A, B) A##B
#define COMBINE(A, B) COMBINE_INNER(A, B)
#define IMPLEMENT_REGISTRY(Type) \
ZSharp::FZRuntimeTypeHandle ZSharp::FZConjugateRegistry_##Type::GetManagedType(const ConjugateType* unmanaged) const \
{ \
	return Alc.GetType(ZSHARP_ENGINE_ASSEMBLY_NAME, FString::Printf(TEXT("%s.%s.%s"), TEXT(ZSHARP_ENGINE_ASSEMBLY_NAME), *TZExportedTypeModule<F##Type>::Get(), *TZExportedTypeName<F##Type>::Get())); \
} \
namespace ZSharp::ZRegularConjugateRegistries_Private \
{ \
	static TZDeclareConjugateRegistry<FZConjugateRegistry_##Type> COMBINE(GDeclare, __LINE__); \
}

IMPLEMENT_REGISTRY(String)
IMPLEMENT_REGISTRY(Name)
IMPLEMENT_REGISTRY(Text)

IMPLEMENT_REGISTRY(WeakObjectPtr)
IMPLEMENT_REGISTRY(SoftObjectPtr)
IMPLEMENT_REGISTRY(LazyObjectPtr)
IMPLEMENT_REGISTRY(StrongObjectPtr)
IMPLEMENT_REGISTRY(SoftClassPtr)
IMPLEMENT_REGISTRY(ScriptInterface)

IMPLEMENT_REGISTRY(FieldPath)

#undef IMPLEMENT_REGISTRY
#undef COMBINE
#undef COMBINE_INNER


