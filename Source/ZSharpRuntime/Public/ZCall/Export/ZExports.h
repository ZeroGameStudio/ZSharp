// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Static/ZStaticExportedClass.h"
#include "StrongObjectPtr.h"
#include "SoftClassPtr.h"
#include "ZCall/Container/ZSelfDescriptiveScriptArray.h"

ZSHARP_DECLARE_EXPORTED_CLASS(FString, UnrealString, Core, 11)
ZSHARP_DECLARE_EXPORTED_CLASS(FName, UnrealName, Core, 12)
ZSHARP_DECLARE_EXPORTED_CLASS(FText, UnrealText, Core, 13)

ZSHARP_DECLARE_EXPORTED_CLASS(FWeakObjectPtr, WeakObjectPtr, CoreUObject, 21)
ZSHARP_DECLARE_EXPORTED_CLASS(FSoftObjectPtr, SoftObjectPtr, CoreUObject, 22)
ZSHARP_DECLARE_EXPORTED_CLASS(FLazyObjectPtr, LazyObjectPtr, CoreUObject, 23)
ZSHARP_DECLARE_EXPORTED_CLASS(FStrongObjectPtr, StrongObjectPtr, CoreUObject, 24)
ZSHARP_DECLARE_EXPORTED_CLASS(FSoftClassPtr, SoftClassPtr, CoreUObject, 25)
ZSHARP_DECLARE_EXPORTED_CLASS(FScriptInterface, ScriptInterface, CoreUObject, 26)

ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(ZSharp::FZSelfDescriptiveScriptArray, 31)


