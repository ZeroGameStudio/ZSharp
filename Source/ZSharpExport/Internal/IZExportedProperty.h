// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZExportedVariable.h"

namespace ZSharp
{
	enum class EZExportedPropertyFlags : uint64
	{
		None = 0,
		Public = 1 << 0,
		Protected = 1 << 1,
		Private = 1 << 2,
		Readable = 1 << 3,
		Writable = 1 << 4,
		ForceCopy = 1 << 5,
	};
	ENUM_CLASS_FLAGS(EZExportedPropertyFlags)
	
	class ZSHARPEXPORT_API IZExportedProperty : public IZExportedVariable
	{
	public:
		virtual EZExportedPropertyFlags GetFlags() const = 0;
		virtual FString GetZCallName() const = 0;
		virtual int32 GetIndex() const = 0;
	public:
		bool HasAnyFlags(EZExportedPropertyFlags flags) const { return !!(GetFlags() & flags); }
		bool HasAllFlags(EZExportedPropertyFlags flags) const { return (GetFlags() & flags) == flags; }
	};
}


