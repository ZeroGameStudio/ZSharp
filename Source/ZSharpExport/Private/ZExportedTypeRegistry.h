// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZExportedTypeRegistry.h"
#include "IZExportedEnum.h"

namespace ZSharp
{
	class FZExportedTypeRegistry : public IZExportedTypeRegistry
	{

	public:
		static FZExportedTypeRegistry& Get();

	public:
		virtual void ForeachExportedClass(TFunctionRef<void(IZExportedClass&)> action) const override;
		virtual void ForeachExportedEnum(TFunctionRef<void(IZExportedEnum&)> action) const override;

	public:
		bool RegisterClass(IZExportedClass* cls);
		bool RegisterEnum(IZExportedEnum* enm);

	private:
		TMap<FString, TUniquePtr<IZExportedEnum>> EnumMap;
		
	};
}


