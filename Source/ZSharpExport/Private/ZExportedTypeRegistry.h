// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZExportedTypeRegistry.h"

namespace ZSharp
{
	class FZExportedTypeRegistry : public IZExportedTypeRegistry
	{

	public:
		virtual void RegisterClass(IZExportedClass* cls) override;
		virtual void RegisterEnum(IZExportedEnum* enm) override;
		virtual void ForeachExportedClass(TFunctionRef<void(IZExportedClass&)> action) const override;
		virtual void ForeachExportedEnum(TFunctionRef<void(IZExportedEnum&)> action) const override;
		
	};
}


