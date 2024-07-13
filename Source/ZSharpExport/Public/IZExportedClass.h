// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZExportedType.h"
#include "ZFullyExportedTypeName.h"

namespace ZSharp
{
	class IZExportedMethod;
	class IZExportedProperty;

	enum class EZExportedClassFlags : uint64
	{
		None = 0,
		Plain = 1 << 0,
		Class = 1 << 1,
		Struct = 1 << 2,
		Interface = 1 << 3,
		Abstract = 1 << 4,
	};
	ENUM_CLASS_FLAGS(EZExportedClassFlags)
	
	class ZSHARPEXPORT_API IZExportedClass : public IZExportedType
	{
	public:
		virtual uint16 GetConjugateRegistryId() const = 0;
		virtual EZExportedClassFlags GetFlags() const = 0;
		virtual FZFullyExportedTypeName GetBaseType() const = 0;
		virtual void ForeachProperty(TFunctionRef<void(const IZExportedProperty&)> action) const = 0;
	public:
		bool HasAnyFlags(EZExportedClassFlags flags) const { return !!(GetFlags() & flags); }
		bool HasAllFlags(EZExportedClassFlags flags) const { return (GetFlags() & flags) == flags; }
	private:
		virtual EZCallBufferSlotType GetSlotType() const override { return EZCallBufferSlotType::Conjugate; }
	};
}


