// Copyright Zero Games. All Rights Reserved.


#include "ZCall/ZCallBuffer.h"

FString ZSharp::ToString(EZCallBufferSlotType type)
{
	static const TMap<EZCallBufferSlotType, FString> GMap =
	{
#define Op(Type) { EZCallBufferSlotType::Type, #Type },
		ZSHARP_FOREACH_ZCALL_BUFFER_SLOT_TYPE()
#undef Op
	};

	const FString* res = GMap.Find(type);
	return res ? *res : "Unknown";
}

ZSharp::FZCallBufferSlot ZSharp::FZCallBufferSlot::FromUInt8(uint8 value)
{
	return { EZCallBufferSlotType::UInt8, { .UInt8 = value } };
}

ZSharp::FZCallBufferSlot ZSharp::FZCallBufferSlot::FromUInt16(uint16 value)
{
	return { EZCallBufferSlotType::UInt16, { .UInt16 = value } };
}

ZSharp::FZCallBufferSlot ZSharp::FZCallBufferSlot::FromUInt32(uint32 value)
{
	return { EZCallBufferSlotType::UInt32, { .UInt32 = value } };
}

ZSharp::FZCallBufferSlot ZSharp::FZCallBufferSlot::FromUInt64(uint64 value)
{
	return { EZCallBufferSlotType::UInt64, { .UInt64 = value } };
}

ZSharp::FZCallBufferSlot ZSharp::FZCallBufferSlot::FromInt8(int8 value)
{
	return { EZCallBufferSlotType::Int8, { .Int8 = value } };
}

ZSharp::FZCallBufferSlot ZSharp::FZCallBufferSlot::FromInt16(int16 value)
{
	return { EZCallBufferSlotType::Int16, { .Int16 = value } };
}

ZSharp::FZCallBufferSlot ZSharp::FZCallBufferSlot::FromInt32(int32 value)
{
	return { EZCallBufferSlotType::Int32, { .Int32 = value } };
}

ZSharp::FZCallBufferSlot ZSharp::FZCallBufferSlot::FromInt64(int64 value)
{
	return { EZCallBufferSlotType::Int64, { .Int64 = value } };
}

ZSharp::FZCallBufferSlot ZSharp::FZCallBufferSlot::FromFloat(float value)
{
	return { EZCallBufferSlotType::Float, { .Float = value } };
}

ZSharp::FZCallBufferSlot ZSharp::FZCallBufferSlot::FromDouble(double value)
{
	return { EZCallBufferSlotType::Double, { .Double = value } };
}

ZSharp::FZCallBufferSlot ZSharp::FZCallBufferSlot::FromBool(bool value)
{
	return { EZCallBufferSlotType::Bool, { .Bool = value } };
}

ZSharp::FZCallBufferSlot ZSharp::FZCallBufferSlot::FromPointer(void* value)
{
	return { EZCallBufferSlotType::Pointer, { .Pointer = value } };
}

ZSharp::FZCallBufferSlot ZSharp::FZCallBufferSlot::FromGCHandle(FZGCHandle value)
{
	return { EZCallBufferSlotType::GCHandle, { .GCHandle = value } };
}

ZSharp::FZCallBufferSlot ZSharp::FZCallBufferSlot::FromConjugate(FZConjugateHandle value)
{
	return { EZCallBufferSlotType::Conjugate, { .Conjugate = value } };
}

uint8 ZSharp::FZCallBufferSlot::ReadUInt8() const
{
	check(Type == EZCallBufferSlotType::UInt8);
	return Value.UInt8;
}

uint16 ZSharp::FZCallBufferSlot::ReadUInt16() const
{
	check(Type == EZCallBufferSlotType::UInt16);
	return Value.UInt16;
}

uint32 ZSharp::FZCallBufferSlot::ReadUInt32() const
{
	check(Type == EZCallBufferSlotType::UInt32);
	return Value.UInt32;
}

uint64 ZSharp::FZCallBufferSlot::ReadUInt64() const
{
	check(Type == EZCallBufferSlotType::UInt64);
	return Value.UInt64;
}

int8 ZSharp::FZCallBufferSlot::ReadInt8() const
{
	check(Type == EZCallBufferSlotType::Int8);
	return Value.Int8;
}

int16 ZSharp::FZCallBufferSlot::ReadInt16() const
{
	check(Type == EZCallBufferSlotType::Int16);
	return Value.Int16;
}

int32 ZSharp::FZCallBufferSlot::ReadInt32() const
{
	check(Type == EZCallBufferSlotType::Int32);
	return Value.Int32;
}

int64 ZSharp::FZCallBufferSlot::ReadInt64() const
{
	check(Type == EZCallBufferSlotType::Int64);
	return Value.Int64;
}

float ZSharp::FZCallBufferSlot::ReadFloat() const
{
	check(Type == EZCallBufferSlotType::Float);
	return Value.Float;
}

double ZSharp::FZCallBufferSlot::ReadDouble() const
{
	check(Type == EZCallBufferSlotType::Double);
	return Value.Double;
}

bool ZSharp::FZCallBufferSlot::ReadBool() const
{
	check(Type == EZCallBufferSlotType::Bool);
	return Value.Bool > 0;
}

void* ZSharp::FZCallBufferSlot::ReadPointer() const
{
	check(Type == EZCallBufferSlotType::Pointer);
	return Value.Pointer;
}

ZSharp::FZGCHandle ZSharp::FZCallBufferSlot::ReadGCHandle() const
{
	check(Type == EZCallBufferSlotType::GCHandle);
	return Value.GCHandle;
}

ZSharp::FZConjugateHandle ZSharp::FZCallBufferSlot::ReadConjugate() const
{
	check(Type == EZCallBufferSlotType::Conjugate);
	return Value.Conjugate;
}

void ZSharp::FZCallBufferSlot::WriteUInt8(uint8 value)
{
	check(Type == EZCallBufferSlotType::UInt8);
	Value.UInt8 = value;
}

void ZSharp::FZCallBufferSlot::WriteUInt16(uint16 value)
{
	check(Type == EZCallBufferSlotType::UInt16);
	Value.UInt16 = value;
}

void ZSharp::FZCallBufferSlot::WriteUInt32(uint32 value)
{
	check(Type == EZCallBufferSlotType::UInt32);
	Value.UInt32 = value;
}

void ZSharp::FZCallBufferSlot::WriteUInt64(uint64 value)
{
	check(Type == EZCallBufferSlotType::UInt64);
	Value.UInt64 = value;
}

void ZSharp::FZCallBufferSlot::WriteInt8(int8 value)
{
	check(Type == EZCallBufferSlotType::Int8);
	Value.Int8 = value;
}

void ZSharp::FZCallBufferSlot::WriteInt16(int16 value)
{
	check(Type == EZCallBufferSlotType::Int16);
	Value.Int16 = value;
}

void ZSharp::FZCallBufferSlot::WriteInt32(int32 value)
{
	check(Type == EZCallBufferSlotType::Int32);
	Value.Int32 = value;
}

void ZSharp::FZCallBufferSlot::WriteInt64(int64 value)
{
	check(Type == EZCallBufferSlotType::Int64);
	Value.Int64 = value;
}

void ZSharp::FZCallBufferSlot::WriteFloat(float value)
{
	check(Type == EZCallBufferSlotType::Float);
	Value.Float = value;
}

void ZSharp::FZCallBufferSlot::WriteDouble(double value)
{
	check(Type == EZCallBufferSlotType::Double);
	Value.Double = value;
}

void ZSharp::FZCallBufferSlot::WriteBool(bool value)
{
	check(Type == EZCallBufferSlotType::Bool);
	Value.Bool = value;
}

void ZSharp::FZCallBufferSlot::WritePointer(void* value)
{
	check(Type == EZCallBufferSlotType::Pointer);
	Value.Pointer = value;
}

void ZSharp::FZCallBufferSlot::WriteGCHandle(FZGCHandle value)
{
	check(Type == EZCallBufferSlotType::GCHandle);
	Value.GCHandle = value;
}

void ZSharp::FZCallBufferSlot::WriteConjugate(FZConjugateHandle value)
{
	check(Type == EZCallBufferSlotType::Conjugate);
	Value.Conjugate = value;
}

ZSharp::FZCallBufferSlot& ZSharp::FZCallBuffer::operator[](int32 index)
{
	check(index >= 0 && index < NumSlots);
	return Slots[index];
}


