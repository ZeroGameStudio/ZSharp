// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZExportedTypeRegistry.h"
#include "IZExportedClass.h"
#include "IZExportedEnum.h"

namespace ZSharp
{
	class FZExportedTypeRegistry : public IZExportedTypeRegistry
	{

	public:
		static FZExportedTypeRegistry& Get();

	public:
		virtual void ForeachExportedClass(TFunctionRef<void(const IZExportedClass&)> action) const override;
		virtual void ForeachExportedEnum(TFunctionRef<void(const IZExportedEnum&)> action) const override;

	public:
		bool RegisterClass(IZExportedClass* cls);
		bool RegisterEnum(IZExportedEnum* enm);

	private:
		TMap<FString, TUniquePtr<IZExportedClass>> ClassMap;
		TMap<FString, TUniquePtr<IZExportedEnum>> EnumMap;
		
	};
}


