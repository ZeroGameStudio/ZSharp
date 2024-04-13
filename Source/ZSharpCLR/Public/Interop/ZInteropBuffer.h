// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZGCHandle.h"

namespace ZSharp
{
	class IZType;
	class IZConjugate;

	enum class EZInteropBufferSlotType : uint8
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
		String,
		Conjugate,
	};

	struct ZSHARPCLR_API FZInteropConjugateBuffer
	{
		FZGCHandle ConjugateType;
		FZGCHandle InnerType;
		FZGCHandle OuterType;
		FZGCHandle Conjugate;
	};

	struct ZSHARPCLR_API FZInteropBufferSlot
	{
		EZInteropBufferSlotType Type;
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
			bool Bool;
			FZGCHandle String;
			FZInteropConjugateBuffer Conjugate;
		};
	};
	
	struct ZSHARPCLR_API FZInteropBuffer
	{
		FZInteropBufferSlot* Slots;
		uint8 NumSlots;
	};
}
