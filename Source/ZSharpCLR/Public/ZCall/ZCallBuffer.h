// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZConjugateHandle.h"
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
	};

	ZSHARPCLR_API FString ToString(EZCallBufferSlotType type);
	
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

	struct FZCallBufferSlot
	{
		EZCallBufferSlotType Type;
		FZCallBufferSlotValue Value;
	};

	// NOTE: Do NOT Construct/Read/Write this directly, use helper functions and macros below.
	struct ZSHARPCLR_API FZCallBuffer
	{
		FZCallBufferSlot* Slots;
		int32 NumSlots;
	};

	ZSHARPCLR_API uint8 ReadUInt8(FZCallBuffer* buffer, int32 index);
	ZSHARPCLR_API uint16 ReadUInt16(FZCallBuffer* buffer, int32 index);
	ZSHARPCLR_API uint32 ReadUInt32(FZCallBuffer* buffer, int32 index);
	ZSHARPCLR_API uint64 ReadUInt64(FZCallBuffer* buffer, int32 index);
	ZSHARPCLR_API int8 ReadInt8(FZCallBuffer* buffer, int32 index);
	ZSHARPCLR_API int16 ReadInt16(FZCallBuffer* buffer, int32 index);
	ZSHARPCLR_API int32 ReadInt32(FZCallBuffer* buffer, int32 index);
	ZSHARPCLR_API int64 ReadInt64(FZCallBuffer* buffer, int32 index);
	ZSHARPCLR_API float ReadFloat(FZCallBuffer* buffer, int32 index);
	ZSHARPCLR_API double ReadDouble(FZCallBuffer* buffer, int32 index);
	ZSHARPCLR_API bool ReadBool(FZCallBuffer* buffer, int32 index);
	ZSHARPCLR_API void* ReadPointer(FZCallBuffer* buffer, int32 index);
	ZSHARPCLR_API FZGCHandle ReadGCHandle(FZCallBuffer* buffer, int32 index);
	ZSHARPCLR_API FZConjugateHandle ReadConjugate(FZCallBuffer* buffer, int32 index);

	ZSHARPCLR_API void WriteUInt8(FZCallBuffer* buffer, int32 index, uint8 value);
	ZSHARPCLR_API void WriteUInt16(FZCallBuffer* buffer, int32 index, uint16 value);
	ZSHARPCLR_API void WriteUInt32(FZCallBuffer* buffer, int32 index, uint32 value);
	ZSHARPCLR_API void WriteUInt64(FZCallBuffer* buffer, int32 index, uint64 value);
	ZSHARPCLR_API void WriteInt8(FZCallBuffer* buffer, int32 index, int8 value);
	ZSHARPCLR_API void WriteInt16(FZCallBuffer* buffer, int32 index, int16 value);
	ZSHARPCLR_API void WriteInt32(FZCallBuffer* buffer, int32 index, int32 value);
	ZSHARPCLR_API void WriteInt64(FZCallBuffer* buffer, int32 index, int64 value);
	ZSHARPCLR_API void WriteFloat(FZCallBuffer* buffer, int32 index, float value);
	ZSHARPCLR_API void WriteDouble(FZCallBuffer* buffer, int32 index, double value);
	ZSHARPCLR_API void WriteBool(FZCallBuffer* buffer, int32 index, bool value);
	ZSHARPCLR_API void WritePointer(FZCallBuffer* buffer, int32 index, void* value);
	ZSHARPCLR_API void WriteGCHandle(FZCallBuffer* buffer, int32 index, FZGCHandle value);
	ZSHARPCLR_API void WriteConjugate(FZCallBuffer* buffer, int32 index, FZConjugateHandle value);
}

namespace ZSharp::ZCallBuffer_Private
{
	template <EZCallBufferSlotType SlotType>
	struct TZCallBufferSlotValueType;

	template <> struct TZCallBufferSlotValueType<EZCallBufferSlotType::UInt8> { using ValueType = uint8; };
	template <> struct TZCallBufferSlotValueType<EZCallBufferSlotType::UInt16> { using ValueType = uint16; };
	template <> struct TZCallBufferSlotValueType<EZCallBufferSlotType::UInt32> { using ValueType = uint32; };
	template <> struct TZCallBufferSlotValueType<EZCallBufferSlotType::UInt64> { using ValueType = uint64; };
	template <> struct TZCallBufferSlotValueType<EZCallBufferSlotType::Int8> { using ValueType = int8; };
	template <> struct TZCallBufferSlotValueType<EZCallBufferSlotType::Int16> { using ValueType = int16; };
	template <> struct TZCallBufferSlotValueType<EZCallBufferSlotType::Int32> { using ValueType = int32; };
	template <> struct TZCallBufferSlotValueType<EZCallBufferSlotType::Int64> { using ValueType = int64; };
	template <> struct TZCallBufferSlotValueType<EZCallBufferSlotType::Float> { using ValueType = float; };
	template <> struct TZCallBufferSlotValueType<EZCallBufferSlotType::Double> { using ValueType = double; };
	template <> struct TZCallBufferSlotValueType<EZCallBufferSlotType::Bool> { using ValueType = bool; };
	template <> struct TZCallBufferSlotValueType<EZCallBufferSlotType::Pointer> { using ValueType = void*; };
	template <> struct TZCallBufferSlotValueType<EZCallBufferSlotType::GCHandle> { using ValueType = FZGCHandle; };
	template <> struct TZCallBufferSlotValueType<EZCallBufferSlotType::Conjugate> { using ValueType = FZConjugateHandle; };
	
	template <EZCallBufferSlotType SlotType>
	struct TZCallBufferSlotFactory
	{
		static FZCallBufferSlot Get(typename TZCallBufferSlotValueType<SlotType>::ValueType value)
		{
			return { SlotType, *reinterpret_cast<FZCallBufferSlotValue*>(&value) };
		}
	};
}

#define ZSHARP_COMBINE(A, B) A##B

#define ZSHARP_STACK_ALLOC_NAMED_ZCALL_BUFFER(BufferVariableName, ...) \
	ZSharp::FZCallBufferSlot ZSHARP_COMBINE(__SLOTS, __LINE__)[] { __VA_ARGS__ }; \
	ZSharp::FZCallBuffer BufferVariableName { ZSHARP_COMBINE(__SLOTS, __LINE__), UE_ARRAY_COUNT(ZSHARP_COMBINE(__SLOTS, __LINE__)) };

#define ZSHARP_STACK_ALLOC_ZCALL_BUFFER(...) ZSHARP_STACK_ALLOC_NAMED_ZCALL_BUFFER(buffer, __VA_ARGS__)

#define ZSHARP_CONSTRUCT_ZCALL_BUFFER_SLOT_UINT8(Value) ZSharp::ZCallBuffer_Private::TZCallBufferSlotFactory<ZSharp::EZCallBufferSlotType::UInt8>::Get(Value)
#define ZSHARP_CONSTRUCT_ZCALL_BUFFER_SLOT_UINT16(Value) ZSharp::ZCallBuffer_Private::TZCallBufferSlotFactory<ZSharp::EZCallBufferSlotType::UInt16>::Get(Value)
#define ZSHARP_CONSTRUCT_ZCALL_BUFFER_SLOT_UINT32(Value) ZSharp::ZCallBuffer_Private::TZCallBufferSlotFactory<ZSharp::EZCallBufferSlotType::UInt32>::Get(Value)
#define ZSHARP_CONSTRUCT_ZCALL_BUFFER_SLOT_UINT64(Value) ZSharp::ZCallBuffer_Private::TZCallBufferSlotFactory<ZSharp::EZCallBufferSlotType::UInt64>::Get(Value)
#define ZSHARP_CONSTRUCT_ZCALL_BUFFER_SLOT_INT8(Value) ZSharp::ZCallBuffer_Private::TZCallBufferSlotFactory<ZSharp::EZCallBufferSlotType::Int8>::Get(Value)
#define ZSHARP_CONSTRUCT_ZCALL_BUFFER_SLOT_INT16(Value) ZSharp::ZCallBuffer_Private::TZCallBufferSlotFactory<ZSharp::EZCallBufferSlotType::Int16>::Get(Value)
#define ZSHARP_CONSTRUCT_ZCALL_BUFFER_SLOT_INT32(Value) ZSharp::ZCallBuffer_Private::TZCallBufferSlotFactory<ZSharp::EZCallBufferSlotType::Int32>::Get(Value)
#define ZSHARP_CONSTRUCT_ZCALL_BUFFER_SLOT_INT64(Value) ZSharp::ZCallBuffer_Private::TZCallBufferSlotFactory<ZSharp::EZCallBufferSlotType::Int64>::Get(Value)
#define ZSHARP_CONSTRUCT_ZCALL_BUFFER_SLOT_FLOAT(Value) ZSharp::ZCallBuffer_Private::TZCallBufferSlotFactory<ZSharp::EZCallBufferSlotType::Float>::Get(Value)
#define ZSHARP_CONSTRUCT_ZCALL_BUFFER_SLOT_DOUBLE(Value) ZSharp::ZCallBuffer_Private::TZCallBufferSlotFactory<ZSharp::EZCallBufferSlotType::Double>::Get(Value)
#define ZSHARP_CONSTRUCT_ZCALL_BUFFER_SLOT_BOOL(Value) ZSharp::ZCallBuffer_Private::TZCallBufferSlotFactory<ZSharp::EZCallBufferSlotType::Bool>::Get(Value)
#define ZSHARP_CONSTRUCT_ZCALL_BUFFER_SLOT_POINTER(Value) ZSharp::ZCallBuffer_Private::TZCallBufferSlotFactory<ZSharp::EZCallBufferSlotType::Pointer>::Get(Value)
#define ZSHARP_CONSTRUCT_ZCALL_BUFFER_SLOT_GCHANDLE(Value) ZSharp::ZCallBuffer_Private::TZCallBufferSlotFactory<ZSharp::EZCallBufferSlotType::GCHandle>::Get(Value)
#define ZSHARP_CONSTRUCT_ZCALL_BUFFER_SLOT_CONJUGATE(Value) ZSharp::ZCallBuffer_Private::TZCallBufferSlotFactory<ZSharp::EZCallBufferSlotType::Conjugate>::Get(Value)


