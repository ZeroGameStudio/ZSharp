// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZExportedType.h"

namespace ZSharp
{
	class IZExportedMethod;

	enum class EZExportedClassFlags : uint64
	{
		None = 0,
		Class = 1 << 0,
		Struct = 1 << 1,
		Interface = 1 << 2,
		Abstract = 1 << 3,
	};
	ENUM_CLASS_FLAGS(EZExportedClassFlags)
	
	class ZSHARPEXPORT_API IZExportedClass : public IZExportedType
	{
	public:
		virtual EZExportedClassFlags GetFlags() const = 0;
		virtual FString GetBaseType() const = 0;
		virtual TArray<FString> GetInterfaces() const = 0;
		virtual void ForeachMethod(TFunctionRef<void(IZExportedMethod&)> action) const = 0;
	public:
		bool HasAnyFlags(EZExportedClassFlags flags) const { return !!(GetFlags() & flags); }
		bool HasAllFlags(EZExportedClassFlags flags) const { return (GetFlags() & flags) == flags; }
	private:
		virtual EZCallBufferSlotType GetSlotType() const override { return EZCallBufferSlotType::Conjugate; }
	};
}


