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

uint8 ZSharp::ReadUInt8(FZCallBuffer* buffer, int32 index)
{
	check(index >= 0 && index < buffer->NumSlots);
	check(buffer->Slots[index].Type == EZCallBufferSlotType::UInt8);

	return buffer->Slots->Value.UInt8;
}

uint16 ZSharp::ReadUInt16(FZCallBuffer* buffer, int32 index)
{
	check(index >= 0 && index < buffer->NumSlots);
	check(buffer->Slots[index].Type == EZCallBufferSlotType::UInt16);

	return buffer->Slots->Value.UInt16;
}

uint32 ZSharp::ReadUInt32(FZCallBuffer* buffer, int32 index)
{
	check(index >= 0 && index < buffer->NumSlots);
	check(buffer->Slots[index].Type == EZCallBufferSlotType::UInt32);

	return buffer->Slots->Value.UInt32;
}

uint64 ZSharp::ReadUInt64(FZCallBuffer* buffer, int32 index)
{
	check(index >= 0 && index < buffer->NumSlots);
	check(buffer->Slots[index].Type == EZCallBufferSlotType::UInt64);

	return buffer->Slots->Value.UInt64;
}

int8 ZSharp::ReadInt8(FZCallBuffer* buffer, int32 index)
{
	check(index >= 0 && index < buffer->NumSlots);
	check(buffer->Slots[index].Type == EZCallBufferSlotType::Int8);

	return buffer->Slots->Value.Int8;
}

int16 ZSharp::ReadInt16(FZCallBuffer* buffer, int32 index)
{
	check(index >= 0 && index < buffer->NumSlots);
	check(buffer->Slots[index].Type == EZCallBufferSlotType::Int16);

	return buffer->Slots->Value.Int16;
}

int32 ZSharp::ReadInt32(FZCallBuffer* buffer, int32 index)
{
	check(index >= 0 && index < buffer->NumSlots);
	check(buffer->Slots[index].Type == EZCallBufferSlotType::Int32);

	return buffer->Slots->Value.Int32;
}

int64 ZSharp::ReadInt64(FZCallBuffer* buffer, int32 index)
{
	check(index >= 0 && index < buffer->NumSlots);
	check(buffer->Slots[index].Type == EZCallBufferSlotType::Int64);

	return buffer->Slots->Value.Int64;
}

float ZSharp::ReadFloat(FZCallBuffer* buffer, int32 index)
{
	check(index >= 0 && index < buffer->NumSlots);
	check(buffer->Slots[index].Type == EZCallBufferSlotType::Float);

	return buffer->Slots->Value.Float;
}

double ZSharp::ReadDouble(FZCallBuffer* buffer, int32 index)
{
	check(index >= 0 && index < buffer->NumSlots);
	check(buffer->Slots[index].Type == EZCallBufferSlotType::Double);

	return buffer->Slots->Value.Double;
}

bool ZSharp::ReadBool(FZCallBuffer* buffer, int32 index)
{
	check(index >= 0 && index < buffer->NumSlots);
	check(buffer->Slots[index].Type == EZCallBufferSlotType::Bool);

	return buffer->Slots->Value.Bool > 0;
}

void* ZSharp::ReadPointer(FZCallBuffer* buffer, int32 index)
{
	check(index >= 0 && index < buffer->NumSlots);
	check(buffer->Slots[index].Type == EZCallBufferSlotType::Pointer);

	return buffer->Slots->Value.Pointer;
}

ZSharp::FZGCHandle ZSharp::ReadGCHandle(FZCallBuffer* buffer, int32 index)
{
	check(index >= 0 && index < buffer->NumSlots);
	check(buffer->Slots[index].Type == EZCallBufferSlotType::GCHandle);

	return buffer->Slots->Value.GCHandle;
}

ZSharp::FZConjugateHandle ZSharp::ReadConjugate(FZCallBuffer* buffer, int32 index)
{
	check(index >= 0 && index < buffer->NumSlots);
	check(buffer->Slots[index].Type == EZCallBufferSlotType::Conjugate);

	return buffer->Slots->Value.Conjugate;
}

void ZSharp::WriteUInt8(FZCallBuffer* buffer, int32 index, uint8 value)
{
	check(index >= 0 && index < buffer->NumSlots);
	check(buffer->Slots[index].Type == EZCallBufferSlotType::UInt8);

	buffer->Slots->Value.UInt8 = value;
}

void ZSharp::WriteUInt16(FZCallBuffer* buffer, int32 index, uint16 value)
{
	check(index >= 0 && index < buffer->NumSlots);
	check(buffer->Slots[index].Type == EZCallBufferSlotType::UInt16);

	buffer->Slots->Value.UInt16 = value;
}

void ZSharp::WriteUInt32(FZCallBuffer* buffer, int32 index, uint32 value)
{
	check(index >= 0 && index < buffer->NumSlots);
	check(buffer->Slots[index].Type == EZCallBufferSlotType::UInt32);

	buffer->Slots->Value.UInt32 = value;
}

void ZSharp::WriteUInt64(FZCallBuffer* buffer, int32 index, uint64 value)
{
	check(index >= 0 && index < buffer->NumSlots);
	check(buffer->Slots[index].Type == EZCallBufferSlotType::UInt64);

	buffer->Slots->Value.UInt64 = value;
}

void ZSharp::WriteInt8(FZCallBuffer* buffer, int32 index, int8 value)
{
	check(index >= 0 && index < buffer->NumSlots);
	check(buffer->Slots[index].Type == EZCallBufferSlotType::Int8);

	buffer->Slots->Value.Int8 = value;
}

void ZSharp::WriteInt16(FZCallBuffer* buffer, int32 index, int16 value)
{
	check(index >= 0 && index < buffer->NumSlots);
	check(buffer->Slots[index].Type == EZCallBufferSlotType::Int16);

	buffer->Slots->Value.Int16 = value;
}

void ZSharp::WriteInt32(FZCallBuffer* buffer, int32 index, int32 value)
{
	check(index >= 0 && index < buffer->NumSlots);
	check(buffer->Slots[index].Type == EZCallBufferSlotType::Int32);

	buffer->Slots->Value.Int32 = value;
}

void ZSharp::WriteInt64(FZCallBuffer* buffer, int32 index, int64 value)
{
	check(index >= 0 && index < buffer->NumSlots);
	check(buffer->Slots[index].Type == EZCallBufferSlotType::Int64);

	buffer->Slots->Value.Int64 = value;
}

void ZSharp::WriteFloat(FZCallBuffer* buffer, int32 index, float value)
{
	check(index >= 0 && index < buffer->NumSlots);
	check(buffer->Slots[index].Type == EZCallBufferSlotType::Float);

	buffer->Slots->Value.Float = value;
}

void ZSharp::WriteDouble(FZCallBuffer* buffer, int32 index, double value)
{
	check(index >= 0 && index < buffer->NumSlots);
	check(buffer->Slots[index].Type == EZCallBufferSlotType::Double);

	buffer->Slots->Value.Double = value;
}

void ZSharp::WriteBool(FZCallBuffer* buffer, int32 index, bool value)
{
	check(index >= 0 && index < buffer->NumSlots);
	check(buffer->Slots[index].Type == EZCallBufferSlotType::Bool);

	buffer->Slots->Value.Bool = value;
}

void ZSharp::WritePointer(FZCallBuffer* buffer, int32 index, void* value)
{
	check(index >= 0 && index < buffer->NumSlots);
	check(buffer->Slots[index].Type == EZCallBufferSlotType::Pointer);

	buffer->Slots->Value.Pointer = value;
}

void ZSharp::WriteGCHandle(FZCallBuffer* buffer, int32 index, FZGCHandle value)
{
	check(index >= 0 && index < buffer->NumSlots);
	check(buffer->Slots[index].Type == EZCallBufferSlotType::GCHandle);

	buffer->Slots->Value.GCHandle = value;
}

void ZSharp::WriteConjugate(FZCallBuffer* buffer, int32 index, FZConjugateHandle value)
{
	check(index >= 0 && index < buffer->NumSlots);
	check(buffer->Slots[index].Type == EZCallBufferSlotType::Conjugate);

	buffer->Slots->Value.Conjugate = value;
}


