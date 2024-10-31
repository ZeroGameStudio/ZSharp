// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Static/ZStaticallyExportedClass.h"

#define ZSHARP_DECLARE_EXPORTED_CLASS(Class, Name, Module, RegistryId) \
ZSHARP_EXPORT_TYPE_NAME_EX(Class, Name) \
ZSHARP_EXPORT_TYPE_MODULE(Class, Module) \
ZSHARP_EXPORT_CONJUGATE_REGISTRY_ID(Class, RegistryId)

#define ZSHARP_BEGIN_EXPORT_CLASS(Class) \
namespace __ZSharpExport_Private \
{ \
	static struct __FZStaticallyExportedClass_##Class : public ZSharp::TZStaticallyExportedClass<Class> \
	{ \
		using ClassClass = Class; \
		using ThisClass = __FZStaticallyExportedClass_##Class; \
		explicit __FZStaticallyExportedClass_##Class() : ZSharp::TZStaticallyExportedClass<Class>(false) \
		{

#define ZSHARP_END_EXPORT_CLASS(Class) \
			static_assert(std::is_same_v<ThisClass, __FZStaticallyExportedClass_##Class>, "Class name doesn't match between BEGIN_EXPORT and END_EXPORT!"); \
			ZSharp::ZStaticallyExportedClass_Private::RegisterClass(this); \
		} \
	}* __GExportedClass_##Class = new std::remove_pointer_t<decltype(__GExportedClass_##Class)>; \
}


