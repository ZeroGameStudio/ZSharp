// Copyright Zero Games. All Rights Reserved.

#pragma once

// NOTE: Keep sync with ZSharp::FZCallBufferSlot in module ZSharpCLR.
#define ZSHARP_FOREACH_ZCALL_BUFFER_SLOT_TYPE() \
	Op(UInt8) \
	Op(UInt16) \
	Op(UInt32) \
	Op(UInt64) \
	Op(Int8) \
	Op(Int16) \
	Op(Int32) \
	Op(Int64) \
	Op(Float) \
	Op(Double) \
	Op(Bool) \
	Op(Pointer) \
	Op(Conjugate)

namespace ZSharp
{
	enum class EZCallBufferSlotType : uint8
	{
#define Op(Type) Type,
		ZSHARP_FOREACH_ZCALL_BUFFER_SLOT_TYPE()
#undef Op
	};

	ZSHARPEXPORT_API FString ToString(EZCallBufferSlotType type);
}


