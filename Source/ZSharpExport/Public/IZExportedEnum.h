// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZExportedType.h"

namespace ZSharp
{
	class ZSHARPEXPORT_API IZExportedEnum : public IZExportedType
	{
	public:
		virtual FString GetUnderlyingType() const = 0;
		virtual void ForeachEnumValue(TFunctionRef<void(FString, FString)> action) const = 0;
	private:
		virtual bool IsStatic() const override final { return true; }
	};
}


