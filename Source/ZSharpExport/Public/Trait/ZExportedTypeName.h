// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZFullyExportedTypeName.h"
#include "Static/ZExportHelper.h"
#include "Trait/ZIsDynamicallyExportableType.h"
#include "Trait/ZUField.h"

namespace ZSharp
{
	template <typename, typename = void>
	struct TZExportedTypeName;

	template <typename T>
	struct TZExportedTypeName<T, std::enable_if_t<TZIsDynamicallyExportableType_V<T>>>
	{
		static FZFullyExportedTypeName Get()
		{
			return FZExportHelper::GetUFieldFullyExportedName(TZUField<T>::Get());
		}
	};
}

#define ZSHARP_EXPORT_TYPE_NAME_EX(Type, Name, Namespace) template<> struct ZSharp::TZExportedTypeName<Type> { static_assert(!ZSharp::TZIsDynamicallyExportableType_V<Type>); static FZFullyExportedTypeName Get() { return { #Namespace, #Name }; } };
#define ZSHARP_EXPORT_TYPE_NAME(Type, Namespace) ZSHARP_EXPORT_TYPE_NAME_EX(Type, Type, Namespace)

ZSHARP_EXPORT_TYPE_NAME(uint8, System)
ZSHARP_EXPORT_TYPE_NAME(uint16, System)
ZSHARP_EXPORT_TYPE_NAME(uint32, System)
ZSHARP_EXPORT_TYPE_NAME(uint64, System)
ZSHARP_EXPORT_TYPE_NAME(int8, System)
ZSHARP_EXPORT_TYPE_NAME(int16, System)
ZSHARP_EXPORT_TYPE_NAME(int32, System)
ZSHARP_EXPORT_TYPE_NAME(int64, System)
ZSHARP_EXPORT_TYPE_NAME(float, System)
ZSHARP_EXPORT_TYPE_NAME(double, System)
ZSHARP_EXPORT_TYPE_NAME(bool, System)

ZSHARP_EXPORT_TYPE_NAME_EX(FString, UnrealString, ZeroGames.ZSharp.UnrealEngine.Core)
ZSHARP_EXPORT_TYPE_NAME_EX(FName, UnrealName, ZeroGames.ZSharp.UnrealEngine.Core)
ZSHARP_EXPORT_TYPE_NAME_EX(FText, UnrealText, ZeroGames.ZSharp.UnrealEngine.Core)

ZSHARP_EXPORT_TYPE_NAME_EX(FFieldPath, FieldPath, ZeroGames.ZSharp.UnrealEngine.CoreUObject)


