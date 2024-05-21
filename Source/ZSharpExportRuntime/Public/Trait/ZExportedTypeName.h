// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZIsDynamicExportableType.h"

namespace ZSharp
{
	template <typename, typename = void>
	struct TZExportedTypeName;

	template <typename T>
	struct TZExportedTypeName<T, std::enable_if_t<TZIsDynamicExportableType_V<T>>>
	{
		static FString Get()
		{
			if constexpr (TZIsUClass_V<T>)
			{
				return T::StaticClass()->GetName();
			}
			else if constexpr (TZIsUScriptStruct_V<T>)
			{
				return TBaseStructure<T>::Get()->GetName();
			}
			else if constexpr (TZIsDynamicExportableEnum_V<T>)
			{
				return T::StaticEnum()->GetName();
			}

			checkNoEntry();
			return {};
		}
	};
}

#define ZSHARP_EXPORT_TYPE_NAME_EX(Type, Name) template<> struct ZSharp::TZExportedTypeName<Type> { static_assert(!ZSharp::TZIsDynamicExportableType_V<Type>); static FString Get() { return #Name; } };
#define ZSHARP_EXPORT_TYPE_NAME(Type) ZSHARP_EXPORT_TYPE_NAME_EX(Type, Type)

ZSHARP_EXPORT_TYPE_NAME(uint8)
ZSHARP_EXPORT_TYPE_NAME(uint16)
ZSHARP_EXPORT_TYPE_NAME(uint32)
ZSHARP_EXPORT_TYPE_NAME(uint64)
ZSHARP_EXPORT_TYPE_NAME(int8)
ZSHARP_EXPORT_TYPE_NAME(int16)
ZSHARP_EXPORT_TYPE_NAME(int32)
ZSHARP_EXPORT_TYPE_NAME(int64)
ZSHARP_EXPORT_TYPE_NAME(float)
ZSHARP_EXPORT_TYPE_NAME(double)
ZSHARP_EXPORT_TYPE_NAME(bool)


