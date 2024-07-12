// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Trait/ZIsDynamicallyExportableType.h"
#include "Reflection/ZReflectionHelper.h"
#include "ZUField.h"

namespace ZSharp
{
	template <typename, typename = void>
	struct TZExportedTypeModule;

	template <typename T>
	struct TZExportedTypeModule<T, std::enable_if_t<TZIsDynamicallyExportableType_V<T>>>
	{
		static FString Get()
		{
			return FZReflectionHelper::GetUFieldModuleName(TZUField<T>::Get());
		}
	};
}

#define ZSHARP_EXPORT_TYPE_MODULE(Type, Module) template<> struct ZSharp::TZExportedTypeModule<Type> { static_assert(!ZSharp::TZIsDynamicallyExportableType_V<Type>); static FString Get() { return #Module; } };

ZSHARP_EXPORT_TYPE_MODULE(FString, Core)
ZSHARP_EXPORT_TYPE_MODULE(FName, Core)
ZSHARP_EXPORT_TYPE_MODULE(FText, Core)

ZSHARP_EXPORT_TYPE_MODULE(FFieldPath, CoreUObject)


