// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZCall/ZCallBuffer.h"
#include "ZIsDynamicExportableType.h"

namespace ZSharp
{
	template <typename, typename = void>
	struct TZExportedTypeZCallBufferSlotType;

	template <typename T>
	struct TZExportedTypeZCallBufferSlotType<T, std::enable_if_t<TIsEnum<T>::Value>>
	{
		static constexpr EZCallBufferSlotType Get()
		{
			return TZExportedTypeZCallBufferSlotType<__underlying_type(T)>::Get();
		}
	};

	template <typename T>
	struct TZExportedTypeZCallBufferSlotType<T, std::enable_if_t<TZIsDynamicExportableClass_V<T>>>
	{
		static constexpr EZCallBufferSlotType Get()
		{
			return EZCallBufferSlotType::Conjugate;
		}
	};
}

#define ZSHARP_EXPORT_SLOT_TYPE(ExportedType, SlotType) template<> struct ZSharp::TZExportedTypeZCallBufferSlotType<ExportedType> { static_assert(!ZSharp::TZIsDynamicExportableType_V<ExportedType>); static constexpr EZCallBufferSlotType Get() { return EZCallBufferSlotType::SlotType; } };

ZSHARP_EXPORT_SLOT_TYPE(uint8, UInt8)
ZSHARP_EXPORT_SLOT_TYPE(uint16, UInt16)
ZSHARP_EXPORT_SLOT_TYPE(uint32, UInt32)
ZSHARP_EXPORT_SLOT_TYPE(uint64, UInt64)
ZSHARP_EXPORT_SLOT_TYPE(int8, Int8)
ZSHARP_EXPORT_SLOT_TYPE(int16, Int16)
ZSHARP_EXPORT_SLOT_TYPE(int32, Int32)
ZSHARP_EXPORT_SLOT_TYPE(int64, Int64)
ZSHARP_EXPORT_SLOT_TYPE(float, Float)
ZSHARP_EXPORT_SLOT_TYPE(double, Double)
ZSHARP_EXPORT_SLOT_TYPE(bool, Bool)


