// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZExportedType.h"

namespace ZSharp
{
	enum class EZExportedEnumFlags : uint64
	{
		None = 0,
		Flags = 1 << 0,
	};
	ENUM_CLASS_FLAGS(EZExportedEnumFlags)
	
	class ZSHARPEXPORT_API IZExportedEnum : public IZExportedType
	{
	public:
		virtual EZExportedEnumFlags GetFlags() const = 0;
		virtual FString GetUnderlyingType() const = 0;
		virtual void ForeachEnumValue(TFunctionRef<void(const FString&, const FString&)> action) const = 0;
	public:
		bool HasAnyFlags(EZExportedEnumFlags flags) const { return !!(GetFlags() & flags); }
		bool HasAllFlags(EZExportedEnumFlags flags) const { return (GetFlags() & flags) == flags; }
	private:
		virtual FString GetOuterExportName() const override { return GetInnerExportName(); }
	};
}


