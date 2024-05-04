// Copyright Zero Games. All Rights Reserved.

#pragma once

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
		virtual void RegisterClass(IZExportedClass* cls) = 0;
		virtual void RegisterEnum(IZExportedEnum* enm) = 0;
		virtual void ForeachExportedClass(TFunctionRef<void(IZExportedClass&)> action) const = 0;
		virtual void ForeachExportedEnum(TFunctionRef<void(IZExportedEnum&)> action) const = 0;
	};
}


