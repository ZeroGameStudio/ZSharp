// Copyright Zero Games. All Rights Reserved.

#pragma once
#include "IZExportedClass.h"

namespace ZSharp
{
	class IZExportedClass;
	class IZExportedEnum;
	
	class ZSHARPEXPORT_API IZExportedTypeRegistry
	{
	public:
		static IZExportedTypeRegistry& Get();
	public:
		virtual ~IZExportedTypeRegistry(){}
	public:
		virtual void ForeachExportedClass(TFunctionRef<void(const IZExportedClass&)> action) const = 0;
		virtual void ForeachExportedEnum(TFunctionRef<void(const IZExportedEnum&)> action) const = 0;
	};
}


