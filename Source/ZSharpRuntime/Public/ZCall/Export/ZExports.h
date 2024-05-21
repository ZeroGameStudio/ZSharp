// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Static/ZStaticExportedClass.h"
#include "StrongObjectPtr.h"
#include "SoftClassPtr.h"

ZSHARP_DECLARE_EXPORTED_CLASS(FString, UnrealString, Core)
ZSHARP_DECLARE_EXPORTED_CLASS(FName, UnrealName, Core)
ZSHARP_DECLARE_EXPORTED_CLASS(FText, UnrealText, Core)

ZSHARP_DECLARE_EXPORTED_CLASS(FWeakObjectPtr, WeakObjectPtr, CoreUObject)
ZSHARP_DECLARE_EXPORTED_CLASS(FSoftObjectPtr, SoftObjectPtr, CoreUObject)
ZSHARP_DECLARE_EXPORTED_CLASS(FLazyObjectPtr, LazyObjectPtr, CoreUObject)
ZSHARP_DECLARE_EXPORTED_CLASS(FStrongObjectPtr, StrongObjectPtr, CoreUObject)
ZSHARP_DECLARE_EXPORTED_CLASS(FSoftClassPtr, SoftClassPtr, CoreUObject)
ZSHARP_DECLARE_EXPORTED_CLASS(FScriptInterface, ScriptInterface, CoreUObject)


