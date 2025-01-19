// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Static/ZStaticallyExportedEnum.h"

#define ZSHARP_DECLARE_EXPORTED_ENUM(Enum, Module, Name) \
ZSHARP_DECLARE_ENGINE_MANAGED_TYPE_INFO(Enum, Module, Name)

#define ZSHARP_BEGIN_EXPORT_ENUM_EX(Enum, bFlags, Name) \
namespace __ZSharpExport_Private \
{ \
	static struct __FZStaticallyExportedEnum_##Name : public ZSharp::TZStaticallyExportedEnum<Enum> \
	{ \
		using EnumClass = Enum; \
		using ThisClass = __FZStaticallyExportedEnum_##Name; \
		explicit __FZStaticallyExportedEnum_##Name() : ZSharp::TZStaticallyExportedEnum<Enum>(bFlags) \
		{

#define ZSHARP_BEGIN_EXPORT_ENUM(Enum, bFlags) ZSHARP_BEGIN_EXPORT_ENUM_EX(Enum, bFlags, Enum)

#define ZSHARP_EXPORT_ENUM_VALUE(Value) AddEnumValue(#Value, EnumClass::Value);

#define ZSHARP_END_EXPORT_ENUM_EX(Enum, Name) \
			static_assert(std::is_same_v<ThisClass, __FZStaticallyExportedEnum_##Name>, "Enum name doesn't match between BEGIN_EXPORT and END_EXPORT!"); \
			ZSharp::ZStaticallyExportedEnum_Private::RegisterEnum(this); \
		} \
	}* __GExportedEnum_##Name = new std::remove_pointer_t<decltype(__GExportedEnum_##Name)>; \
}

#define ZSHARP_END_EXPORT_ENUM(Enum) ZSHARP_END_EXPORT_ENUM_EX(Enum, Enum)


