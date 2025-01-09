// Copyright Zero Games. All Rights Reserved.

#include "ZEnumPropertyVisitor.h"

#include "ZSharpRuntimeLogChannels.h"
#include "ZCall/ZCallBuffer.h"

void ZSharp::FZEnumPropertyVisitor::GetValue(const void* src, FZCallBufferSlot& dest) const
{
	if (const auto uint8Prop = CastField<FByteProperty>(UnderlyingNumericProperty))
	{
		const uint8 value = uint8Prop->GetPropertyValue(src);
		dest.WriteInt64(value);
	}
	else if (const auto uint16Prop = CastField<FUInt16Property>(UnderlyingNumericProperty))
	{
		const uint16 value = uint16Prop->GetPropertyValue(src);
		dest.WriteInt64(value);
	}
	else if (const auto uint32Prop = CastField<FUInt32Property>(UnderlyingNumericProperty))
	{
		const uint32 value = uint32Prop->GetPropertyValue(src);
		dest.WriteInt64(value);
	}
	else if (const auto uint64Prop = CastField<FUInt64Property>(UnderlyingNumericProperty))
	{
		const uint64 value = uint64Prop->GetPropertyValue(src);
		check(value <= MAX_int64);
		dest.WriteInt64(value);
	}
	else if (const auto int8Prop = CastField<FInt8Property>(UnderlyingNumericProperty))
	{
		const int8 value = int8Prop->GetPropertyValue(src);
		dest.WriteInt64(value);
	}
	else if (const auto int16Prop = CastField<FInt16Property>(UnderlyingNumericProperty))
	{
		const int16 value = int16Prop->GetPropertyValue(src);
		dest.WriteInt64(value);
	}
	else if (const auto int32Prop = CastField<FIntProperty>(UnderlyingNumericProperty))
	{
		const int32 value = int32Prop->GetPropertyValue(src);
		dest.WriteInt64(value);
	}
	else if (const auto int64Prop = CastField<FInt64Property>(UnderlyingNumericProperty))
	{
		const int64 value = int64Prop->GetPropertyValue(src);
		dest.WriteInt64(value);
	}
	else
	{
		checkNoEntry();
		UE_LOG(LogZSharpRuntime, Fatal, TEXT("Property type [%s] is not supported!!!"), *UnderlyingNumericProperty->GetClass()->GetName());
	}
}

void ZSharp::FZEnumPropertyVisitor::SetValue(void* dest, const FZCallBufferSlot& src) const
{
	if (UnderlyingNumericProperty->IsA<FByteProperty>())
	{
		const auto value = static_cast<uint8>(src.ReadInt64());
		UnderlyingNumericProperty->CopySingleValue(dest, &value);
	}
	else if (UnderlyingNumericProperty->IsA<FUInt16Property>())
	{
		const auto value = static_cast<uint16>(src.ReadInt64());
		UnderlyingNumericProperty->CopySingleValue(dest, &value);
	}
	else if (UnderlyingNumericProperty->IsA<FUInt32Property>())
	{
		const auto value = static_cast<uint32>(src.ReadInt64());
		UnderlyingNumericProperty->CopySingleValue(dest, &value);
	}
	else if (UnderlyingNumericProperty->IsA<FUInt64Property>())
	{
		const int64 raw = src.ReadInt64();
		check(raw >= 0);
		const auto value = static_cast<uint64>(raw);
		UnderlyingNumericProperty->CopySingleValue(dest, &value);
	}
	else if (UnderlyingNumericProperty->IsA<FInt8Property>())
	{
		const auto value = static_cast<int8>(src.ReadInt64());
		UnderlyingNumericProperty->CopySingleValue(dest, &value);
	}
	else if (UnderlyingNumericProperty->IsA<FInt16Property>())
	{
		const auto value = static_cast<int16>(src.ReadInt64());
		UnderlyingNumericProperty->CopySingleValue(dest, &value);
	}
	else if (UnderlyingNumericProperty->IsA<FIntProperty>())
	{
		const auto value = static_cast<int32>(src.ReadInt64());
		UnderlyingNumericProperty->CopySingleValue(dest, &value);
	}
	else if (UnderlyingNumericProperty->IsA<FInt64Property>())
	{
		const auto value = src.ReadInt64();
		UnderlyingNumericProperty->CopySingleValue(dest, &value);
	}
	else
	{
		checkNoEntry();
		UE_LOG(LogZSharpRuntime, Fatal, TEXT("Property type [%s] is not supported!!!"), *UnderlyingNumericProperty->GetClass()->GetName());
	}
}


