// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZGCHandle.h"

namespace ZSharp
{
	enum class EZCallBufferSlotType : uint8
	{
		UInt8,
		UInt16,
		UInt32,
		UInt64,
		Int8,
		Int16,
		Int32,
		Int64,
		Float,
		Double,
		Bool,
		Conjugate,
	};

	struct ZSHARPCLR_API FZCallBufferSlot
	{
		EZCallBufferSlotType Type;
		union
		{
			uint8 UInt8;
			uint16 UInt16;
			uint32 UInt32;
			uint64 UInt64;
			int8 Int8;
			int16 Int16;
			int32 Int32;
			int64 Int64;
			float Float;
			double Double;
			uint8 Bool;
			FZGCHandle Conjugate;
		} Content;
	};
	
	struct ZSHARPCLR_API FZCallBuffer
	{
		FZCallBufferSlot* Slots;
		uint8 NumSlots;
	};
}
