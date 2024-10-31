// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Static/ZStaticallyExportedEnum.h"

#define ZSHARP_DECLARE_EXPORTED_ENUM(Enum, Name, Module) \
ZSHARP_EXPORT_TYPE_NAME_EX(Enum, Name) \
ZSHARP_EXPORT_TYPE_MODULE(Enum, Module)

#define ZSHARP_BEGIN_EXPORT_ENUM(Enum, bFlags) \
namespace __ZSharpExport_Private \
{ \
	static struct __FZStaticallyExportedEnum_##Enum : public ZSharp::TZStaticallyExportedEnum<Enum> \
	{ \
		using EnumClass = Enum; \
		using ThisClass = __FZStaticallyExportedEnum_##Enum; \
		explicit __FZStaticallyExportedEnum_##Enum() : ZSharp::TZStaticallyExportedEnum<Enum>(bFlags) \
		{

#define ZSHARP_EXPORT_ENUM_VALUE(Value) AddEnumValue(#Value, EnumClass::Value);

#define ZSHARP_END_EXPORT_ENUM(Enum) \
			static_assert(std::is_same_v<ThisClass, __FZStaticallyExportedEnum_##Enum>, "Enum name doesn't match between BEGIN_EXPORT and END_EXPORT!"); \
			ZSharp::ZStaticallyExportedEnum_Private::RegisterEnum(this); \
		} \
	}* __GExportedEnum_##Enum = new std::remove_pointer_t<decltype(__GExportedEnum_##Enum)>; \
}


