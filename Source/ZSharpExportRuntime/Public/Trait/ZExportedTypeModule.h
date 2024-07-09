// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZIsDynamicExportableType.h"
#include "ZSharpExportHelpers.h"
#include "ZUField.h"
#include "StrongObjectPtr.h"
#include "SoftClassPtr.h"

namespace ZSharp
{
	template <typename, typename = void>
	struct TZExportedTypeModule;

	template <typename T>
	struct TZExportedTypeModule<T, std::enable_if_t<TZIsDynamicExportableType_V<T>>>
	{
		static FString Get()
		{
			return FZSharpExportHelpers::GetUFieldModuleName(TZUField<T>::Get());
		}
	};
}

#define ZSHARP_EXPORT_TYPE_MODULE(Type, Module) template<> struct ZSharp::TZExportedTypeModule<Type> { static_assert(!ZSharp::TZIsDynamicExportableType_V<Type>); static FString Get() { return #Module; } };

ZSHARP_EXPORT_TYPE_MODULE(FString, Core)
ZSHARP_EXPORT_TYPE_MODULE(FName, Core)
ZSHARP_EXPORT_TYPE_MODULE(FText, Core)

ZSHARP_EXPORT_TYPE_MODULE(FSoftObjectPtr, CoreUObject)
ZSHARP_EXPORT_TYPE_MODULE(FSoftClassPtr, CoreUObject)
ZSHARP_EXPORT_TYPE_MODULE(FWeakObjectPtr, CoreUObject)
ZSHARP_EXPORT_TYPE_MODULE(FLazyObjectPtr, CoreUObject)
ZSHARP_EXPORT_TYPE_MODULE(FStrongObjectPtr, CoreUObject)
ZSHARP_EXPORT_TYPE_MODULE(FScriptInterface, CoreUObject)

ZSHARP_EXPORT_TYPE_MODULE(FFieldPath, CoreUObject)


