// Copyright Zero Games. All Rights Reserved.

#include "ZNumericPropertyVisitor.h"

#include "ZSharpRuntimeLogChannels.h"
#include "ZCall/ZCallBuffer.h"

ZSharp::FZNumericPropertyVisitor::FZNumericPropertyVisitor(const FNumericProperty* underlyingProperty)
	: FZPrimitivePropertyVisitorBase(underlyingProperty)
	, Enum(underlyingProperty->GetIntPropertyEnum())
{
	check(!Enum || UnderlyingProperty->IsA<FByteProperty>());
}

void ZSharp::FZNumericPropertyVisitor::GetValue(const void* src, FZCallBufferSlot& dest) const
{
	if (const auto uint8Prop = CastField<FByteProperty>(UnderlyingProperty))
	{
		const uint8 value = uint8Prop->GetPropertyValue(src);
		Enum ? dest.WriteInt64(value) : dest.WriteUInt8(uint8Prop->GetPropertyValue(src));
	}
	else if (const auto uint16Prop = CastField<FUInt16Property>(UnderlyingProperty))
	{
		dest.WriteUInt16(uint16Prop->GetPropertyValue(src));
	}
	else if (const auto uint32Prop = CastField<FUInt32Property>(UnderlyingProperty))
	{
		dest.WriteUInt32(uint32Prop->GetPropertyValue(src));
	}
	else if (const auto uint64Prop = CastField<FUInt64Property>(UnderlyingProperty))
	{
		dest.WriteUInt64(uint64Prop->GetPropertyValue(src));
	}
	else if (const auto int8Prop = CastField<FInt8Property>(UnderlyingProperty))
	{
		dest.WriteInt8(int8Prop->GetPropertyValue(src));
	}
	else if (const auto int16Prop = CastField<FInt16Property>(UnderlyingProperty))
	{
		dest.WriteInt16(int16Prop->GetPropertyValue(src));
	}
	else if (const auto int32Prop = CastField<FIntProperty>(UnderlyingProperty))
	{
		dest.WriteInt32(int32Prop->GetPropertyValue(src));
	}
	else if (const auto int64Prop = CastField<FInt64Property>(UnderlyingProperty))
	{
		dest.WriteInt64(int64Prop->GetPropertyValue(src));
	}
	else if (const auto floatProp = CastField<FFloatProperty>(UnderlyingProperty))
	{
		dest.WriteFloat(floatProp->GetPropertyValue(src));
	}
	else if (const auto doubleProp = CastField<FDoubleProperty>(UnderlyingProperty))
	{
		dest.WriteDouble(doubleProp->GetPropertyValue(src));
	}
	else
	{
		checkNoEntry();
		UE_LOG(LogZSharpRuntime, Fatal, TEXT("Property type [%s] is not supported!!!"), *UnderlyingProperty->GetClass()->GetName());
	}
}

void ZSharp::FZNumericPropertyVisitor::SetValue(void* dest, const FZCallBufferSlot& src) const
{
	if (UnderlyingProperty->IsA<FByteProperty>())
	{
		const uint8 value = Enum ? static_cast<uint8>(src.ReadInt64()) : src.ReadUInt8();
		UnderlyingProperty->CopySingleValue(dest, &value);
	}
	else if (UnderlyingProperty->IsA<FUInt16Property>())
	{
		const uint16 value = src.ReadUInt16();
		UnderlyingProperty->CopySingleValue(dest, &value);
	}
	else if (UnderlyingProperty->IsA<FUInt32Property>())
	{
		const uint32 value = src.ReadUInt32();
		UnderlyingProperty->CopySingleValue(dest, &value);
	}
	else if (UnderlyingProperty->IsA<FUInt64Property>())
	{
		const uint64 value = src.ReadUInt64();
		UnderlyingProperty->CopySingleValue(dest, &value);
	}
	else if (UnderlyingProperty->IsA<FInt8Property>())
	{
		const int8 value = src.ReadInt8();
		UnderlyingProperty->CopySingleValue(dest, &value);
	}
	else if (UnderlyingProperty->IsA<FInt16Property>())
	{
		const int16 value = src.ReadInt16();
		UnderlyingProperty->CopySingleValue(dest, &value);
	}
	else if (UnderlyingProperty->IsA<FIntProperty>())
	{
		const int32 value = src.ReadInt32();
		UnderlyingProperty->CopySingleValue(dest, &value);
	}
	else if (UnderlyingProperty->IsA<FInt64Property>())
	{
		const int64 value = src.ReadInt64();
		UnderlyingProperty->CopySingleValue(dest, &value);
	}
	else if (UnderlyingProperty->IsA<FFloatProperty>())
	{
		const float value = src.ReadFloat();
		UnderlyingProperty->CopySingleValue(dest, &value);
	}
	else if (UnderlyingProperty->IsA<FDoubleProperty>())
	{
		const double value = src.ReadDouble();
		UnderlyingProperty->CopySingleValue(dest, &value);
	}
	else
	{
		checkNoEntry();
		UE_LOG(LogZSharpRuntime, Fatal, TEXT("Property type [%s] is not supported!!!"), *UnderlyingProperty->GetClass()->GetName());
	}
}


