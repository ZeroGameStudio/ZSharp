// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZExportedVariable.h"

namespace ZSharp
{
	enum class EZExportedParameterFlags : uint64
	{
		None = 0,
		In = 1 << 0,
		Out = 1 << 1,
		Return = 1 << 2,
	};
	ENUM_CLASS_FLAGS(EZExportedParameterFlags)
	
	class ZSHARPEXPORT_API IZExportedParameter : public IZExportedVariable
	{
	public:
		virtual EZExportedParameterFlags GetFlags() const = 0;
	public:
		bool HasAnyFlags(EZExportedParameterFlags flags) const { return !!(GetFlags() & flags); }
		bool HasAllFlags(EZExportedParameterFlags flags) const { return (GetFlags() & flags) == flags; }
	};
}


