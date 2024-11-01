// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Conjugate/ZDeclareConjugateRegistry.h"

#define ZSHARP_DECLARE_CONJUGATE_REGISTRY_COMBINE_INNER(A, B, C) A##B##C
#define ZSHARP_DECLARE_CONJUGATE_REGISTRY_COMBINE(A, B, C) ZSHARP_DECLARE_CONJUGATE_REGISTRY_COMBINE_INNER(A, B, C)

/**
 * Declares a conjugate registry to automatically register to Master ALC.
 * Generally you don't need to care about this
 * because Z# has implemented necessary conjugates for interacting with Unreal Engine.
 * 
 * @param RegistryType Type of the conjugate registry.
 * @param RegistryName An optional name only used to build variable name.
 */
#define ZSHARP_DECLARE_CONJUGATE_REGISTRY_EX(RegistryType, RegistryName) namespace ZSharp::__ZDeclareConjugateRegistry_Private { static TZDeclareConjugateRegistry<RegistryType> ZSHARP_DECLARE_CONJUGATE_REGISTRY_COMBINE(__GDeclareConjugateRegistry, RegistryName, __LINE__); }
#define ZSHARP_DECLARE_CONJUGATE_REGISTRY(RegistryType) ZSHARP_DECLARE_CONJUGATE_REGISTRY_EX(RegistryType, RegistryType)


