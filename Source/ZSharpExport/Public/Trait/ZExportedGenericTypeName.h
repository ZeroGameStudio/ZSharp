// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZFullyExportedTypeName.h"
#include "Reflection/Wrapper/ZSelfDescriptiveScriptArray.h"
#include "Trait/ZIsDynamicallyExportableType.h"

using FSubclassOf = TSubclassOf<UObject>;
using FSoftClassPtr = TSoftClassPtr<UObject>;
using FStrongObjectPtr = TStrongObjectPtr<UObject>;

namespace ZSharp
{
	template <typename, typename = void>
	struct TZExportedGenericTypeName;
}

#define ZSHARP_EXPORT_GENERIC_TYPE_NAME_EX(Type, Name, Namespace) template<> struct ZSharp::TZExportedGenericTypeName<Type> { static_assert(!ZSharp::TZIsDynamicallyExportableType_V<Type>); static FZFullyExportedTypeName Get() { return { #Namespace, #Name }; } };
#define ZSHARP_EXPORT_GENERIC_TYPE_NAME(Type, Namespace) ZSHARP_EXPORT_TYPE_NAME_EX(Type, Type, Namespace)

ZSHARP_EXPORT_GENERIC_TYPE_NAME_EX(ZSharp::FZSelfDescriptiveScriptArray, UnrealArray, ZeroGames.ZSharp.UnrealEngine.Core)
// ZSHARP_EXPORT_GENERIC_TYPE_NAME_EX(ZSharp::FZSelfDescriptiveScriptSet, UnrealSet, ZeroGames.ZSharp.UnrealEngine.Core)
// ZSHARP_EXPORT_GENERIC_TYPE_NAME_EX(ZSharp::FZSelfDescriptiveScriptMap, UnrealMap, ZeroGames.ZSharp.UnrealEngine.Core)
// ZSHARP_EXPORT_GENERIC_TYPE_NAME_EX(ZSharp::FZSelfDescriptiveScriptOptional, UnrealOptional, ZeroGames.ZSharp.UnrealEngine.Core)

ZSHARP_EXPORT_GENERIC_TYPE_NAME_EX(FSubclassOf, SubclassOf, ZeroGames.ZSharp.UnrealEngine.CoreUObject)
ZSHARP_EXPORT_GENERIC_TYPE_NAME_EX(FSoftClassPtr, SoftClassPtr, ZeroGames.ZSharp.UnrealEngine.CoreUObject)
ZSHARP_EXPORT_GENERIC_TYPE_NAME_EX(FSoftObjectPtr, SoftObjectPtr, ZeroGames.ZSharp.UnrealEngine.CoreUObject)
ZSHARP_EXPORT_GENERIC_TYPE_NAME_EX(FWeakObjectPtr, WeakObjectPtr, ZeroGames.ZSharp.UnrealEngine.CoreUObject)
ZSHARP_EXPORT_GENERIC_TYPE_NAME_EX(FLazyObjectPtr, LazyObjectPtr, ZeroGames.ZSharp.UnrealEngine.CoreUObject)
ZSHARP_EXPORT_GENERIC_TYPE_NAME_EX(FScriptInterface, ScriptInterface, ZeroGames.ZSharp.UnrealEngine.CoreUObject)
ZSHARP_EXPORT_GENERIC_TYPE_NAME_EX(FStrongObjectPtr, StrongObjectPtr, ZeroGames.ZSharp.UnrealEngine.CoreUObject)


