// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZExportedMember.h"
#include "IZExportedParameter.h"

namespace ZSharp
{
	enum class EZExportedMethodFlags : uint64
	{
		None = 0,
		Public = 1 << 0,
		Protected = 1 << 1,
		Private = 1 << 2,
		Static = 1 << 3,
		Virtual = 1 << 4,
		Abstract = 1 << 5,
	};
	ENUM_CLASS_FLAGS(EZExportedMethodFlags)

	class ZSHARPEXPORT_API IZExportedMethod : public IZExportedMember
	{
	public:
		virtual EZExportedMethodFlags GetFlags() const = 0;
		virtual FString GetZCallName() const = 0;
		virtual FZFullyExportedTypeName GetOwnerInterface() const = 0;
		virtual void ForeachParameter(TFunctionRef<void(const IZExportedParameter&)> action) const = 0;
	public:
		bool HasAnyFlags(EZExportedMethodFlags flags) const { return !!(GetFlags() & flags); }
		bool HasAllFlags(EZExportedMethodFlags flags) const { return (GetFlags() & flags) == flags; }
	};
}


