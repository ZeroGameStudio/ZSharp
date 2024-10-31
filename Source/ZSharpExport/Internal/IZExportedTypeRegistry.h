// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	class IZExportedClass;
	class IZExportedEnum;
	class IZExportedDelegate;
	
	class IZExportedTypeRegistry
	{
	public:
		ZSHARPEXPORT_API static IZExportedTypeRegistry& Get();
	public:
		virtual void ForeachExportedClass(TFunctionRef<void(const IZExportedClass&)> action) const = 0;
		virtual void ForeachExportedEnum(TFunctionRef<void(const IZExportedEnum&)> action) const = 0;
		virtual void ForeachExportedDelegate(TFunctionRef<void(const IZExportedDelegate&)> action) const = 0;
	public:
		ZSHARPEXPORT_API void ExportDynamicTypes(const TArray<FString>& assemblies);
	public:
		virtual ~IZExportedTypeRegistry(){}
	};
}


