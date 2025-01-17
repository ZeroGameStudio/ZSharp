// Copyright Zero Games. All Rights Reserved.

#include "ZEnumPropertyVisitor.h"

#include "ZSharpRuntimeLogChannels.h"
#include "ZCall/ZCallBuffer.h"

void ZSharp::FZEnumPropertyVisitor::GetValue(const void* src, FZCallBufferSlot& dest) const
{
	if (auto uint8Prop = CastField<const FByteProperty>(UnderlyingNumericProperty))
	{
		const uint8 value = uint8Prop->GetPropertyValue(src);
		dest.WriteInt64(value);
	}
	else if (auto uint16Prop = CastField<const FUInt16Property>(UnderlyingNumericProperty))
	{
		const uint16 value = uint16Prop->GetPropertyValue(src);
		dest.WriteInt64(value);
	}
	else if (auto uint32Prop = CastField<const FUInt32Property>(UnderlyingNumericProperty))
	{
		const uint32 value = uint32Prop->GetPropertyValue(src);
		dest.WriteInt64(value);
	}
	else if (auto uint64Prop = CastField<const FUInt64Property>(UnderlyingNumericProperty))
	{
		const uint64 value = uint64Prop->GetPropertyValue(src);
		check(value <= MAX_int64);
		dest.WriteInt64(value);
	}
	else if (auto int8Prop = CastField<const FInt8Property>(UnderlyingNumericProperty))
	{
		const int8 value = int8Prop->GetPropertyValue(src);
		dest.WriteInt64(value);
	}
	else if (auto int16Prop = CastField<const FInt16Property>(UnderlyingNumericProperty))
	{
		const int16 value = int16Prop->GetPropertyValue(src);
		dest.WriteInt64(value);
	}
	else if (auto int32Prop = CastField<const FIntProperty>(UnderlyingNumericProperty))
	{
		const int32 value = int32Prop->GetPropertyValue(src);
		dest.WriteInt64(value);
	}
	else if (auto int64Prop = CastField<const FInt64Property>(UnderlyingNumericProperty))
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
		auto value = static_cast<const uint8>(src.ReadInt64());
		UnderlyingNumericProperty->CopySingleValue(dest, &value);
	}
	else if (UnderlyingNumericProperty->IsA<FUInt16Property>())
	{
		auto value = static_cast<const uint16>(src.ReadInt64());
		UnderlyingNumericProperty->CopySingleValue(dest, &value);
	}
	else if (UnderlyingNumericProperty->IsA<FUInt32Property>())
	{
		auto value = static_cast<const uint32>(src.ReadInt64());
		UnderlyingNumericProperty->CopySingleValue(dest, &value);
	}
	else if (UnderlyingNumericProperty->IsA<FUInt64Property>())
	{
		const int64 raw = src.ReadInt64();
		check(raw >= 0);
		auto value = static_cast<const uint64>(raw);
		UnderlyingNumericProperty->CopySingleValue(dest, &value);
	}
	else if (UnderlyingNumericProperty->IsA<FInt8Property>())
	{
		auto value = static_cast<const int8>(src.ReadInt64());
		UnderlyingNumericProperty->CopySingleValue(dest, &value);
	}
	else if (UnderlyingNumericProperty->IsA<FInt16Property>())
	{
		auto value = static_cast<const int16>(src.ReadInt64());
		UnderlyingNumericProperty->CopySingleValue(dest, &value);
	}
	else if (UnderlyingNumericProperty->IsA<FIntProperty>())
	{
		auto value = static_cast<const int32>(src.ReadInt64());
		UnderlyingNumericProperty->CopySingleValue(dest, &value);
	}
	else if (UnderlyingNumericProperty->IsA<FInt64Property>())
	{
		auto value = src.ReadInt64();
		UnderlyingNumericProperty->CopySingleValue(dest, &value);
	}
	else
	{
		checkNoEntry();
		UE_LOG(LogZSharpRuntime, Fatal, TEXT("Property type [%s] is not supported!!!"), *UnderlyingNumericProperty->GetClass()->GetName());
	}
}


