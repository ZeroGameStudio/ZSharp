// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Conjugate/ZConjugateHandle.h"
#include "Interop/ZGCHandle.h"

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
	Op(GCHandle) \
	Op(Conjugate)

namespace ZSharp
{
	enum class EZCallBufferSlotType : uint8
	{
#define Op(Type) Type,
		ZSHARP_FOREACH_ZCALL_BUFFER_SLOT_TYPE()
#undef Op

		Uninitialized = 0xFF,
	};

	ZSHARPCORE_API FString ToString(EZCallBufferSlotType type);
	
	union FZCallBufferSlotValue
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
		void* Pointer;
		FZGCHandle GCHandle;
		FZConjugateHandle Conjugate;
	};

	struct ZSHARPCORE_API FZCallBufferSlot
	{
		
		static FZCallBufferSlot FromUInt8(uint8 value);
		static FZCallBufferSlot FromUInt16(uint16 value);
		static FZCallBufferSlot FromUInt32(uint32 value);
		static FZCallBufferSlot FromUInt64(uint64 value);
		static FZCallBufferSlot FromInt8(int8 value);
		static FZCallBufferSlot FromInt16(int16 value);
		static FZCallBufferSlot FromInt32(int32 value);
		static FZCallBufferSlot FromInt64(int64 value);
		static FZCallBufferSlot FromFloat(float value);
		static FZCallBufferSlot FromDouble(double value);
		static FZCallBufferSlot FromBool(bool value);
		static FZCallBufferSlot FromPointer(void* value);
		static FZCallBufferSlot FromGCHandle(FZGCHandle value);
		static FZCallBufferSlot FromConjugate(FZConjugateHandle value);
	
		uint8 ReadUInt8() const;
		uint16 ReadUInt16() const;
		uint32 ReadUInt32() const;
		uint64 ReadUInt64() const;
		int8 ReadInt8() const;
		int16 ReadInt16() const;
		int32 ReadInt32() const;
		int64 ReadInt64() const;
		float ReadFloat() const;
		double ReadDouble() const;
		bool ReadBool() const;
		void* ReadPointer() const;
		FZGCHandle ReadGCHandle() const;
		FZConjugateHandle ReadConjugate() const;
		
		void WriteUInt8(uint8 value);
		void WriteUInt16(uint16 value);
		void WriteUInt32(uint32 value);
		void WriteUInt64(uint64 value);
		void WriteInt8(int8 value);
		void WriteInt16(int16 value);
		void WriteInt32(int32 value);
		void WriteInt64(int64 value);
		void WriteFloat(float value);
		void WriteDouble(double value);
		void WriteBool(bool value);
		void WritePointer(const void* value);
		void WriteGCHandle(FZGCHandle value);
		void WriteConjugate(FZConjugateHandle value);
		
		EZCallBufferSlotType Type;
		FZCallBufferSlotValue Value;
		
	};

	// NOTE: Do NOT Construct/Read/Write this directly, use helper functions and macros below.
	struct ZSHARPCORE_API FZCallBuffer
	{
		
		FZCallBufferSlot& operator[](int32 index);
		
		FZCallBufferSlot* Slots;
		int32 NumSlots;
		
	};
}

#define ZSHARP_ZCALL_BUFFER_COMBINE_INNER(A, B) A##B
#define ZSHARP_ZCALL_BUFFER_COMBINE(A, B) ZSHARP_ZCALL_BUFFER_COMBINE_INNER(A, B)

#define ZSHARP_STACK_ALLOC_NAMED_ZCALL_BUFFER(BufferVariableName, ...) \
	ZSharp::FZCallBufferSlot ZSHARP_ZCALL_BUFFER_COMBINE(__SLOTS, __LINE__)[] { __VA_ARGS__ }; \
	ZSharp::FZCallBuffer BufferVariableName { ZSHARP_ZCALL_BUFFER_COMBINE(__SLOTS, __LINE__), UE_ARRAY_COUNT(ZSHARP_ZCALL_BUFFER_COMBINE(__SLOTS, __LINE__)) };

#define ZSHARP_STACK_ALLOC_ZCALL_BUFFER(...) ZSHARP_STACK_ALLOC_NAMED_ZCALL_BUFFER(buffer, __VA_ARGS__)


