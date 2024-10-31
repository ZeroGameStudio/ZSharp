// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZExportedType.h"

namespace ZSharp
{
	class IZExportedParameter;
	
	enum class EZExportedDelegateFlags : uint64
	{
		None = 0,
		Multicast = 1 << 0,
		Sparse = 1 << 1,
	};
	ENUM_CLASS_FLAGS(EZExportedDelegateFlags)
	
	class ZSHARPEXPORT_API IZExportedDelegate : public IZExportedType
	{
	public:
		virtual EZExportedDelegateFlags GetFlags() const = 0;
		virtual void ForeachParameter(TFunctionRef<void(const IZExportedParameter&)> action) const = 0;
	public:
		bool HasAnyFlags(EZExportedDelegateFlags flags) const { return !!(GetFlags() & flags); }
		bool HasAllFlags(EZExportedDelegateFlags flags) const { return (GetFlags() & flags) == flags; }
	private:
		virtual EZCallBufferSlotType GetSlotType() const override { return EZCallBufferSlotType::Conjugate; }
	};
}


