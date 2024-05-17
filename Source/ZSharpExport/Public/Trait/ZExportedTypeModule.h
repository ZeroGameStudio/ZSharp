// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZIsDynamicExportableType.h"
#include "ZSharpExportHelpers.h"
#include "ZUField.h"

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


