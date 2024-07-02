// Copyright Zero Games. All Rights Reserved.


#include "ZPrimitivePropertyVisitor.h"

#include "ZCall/ZCallBuffer.h"

void ZSharp::FZPrimitivePropertyVisitor::GetValue_InContainer(const void* src, FZCallBufferSlot& dest) const
{
	const auto writeNumber = [src, &dest](const FNumericProperty* prop)
	{
		if (prop->IsA<FByteProperty>())
		{
			dest.WriteUInt8(*prop->ContainerPtrToValuePtr<uint8>(src));
		}
		else if (prop->IsA<FUInt16Property>())
		{
			dest.WriteUInt16(*prop->ContainerPtrToValuePtr<uint16>(src));
		}
		else if (prop->IsA<FUInt32Property>())
		{
			dest.WriteUInt32(*prop->ContainerPtrToValuePtr<uint32>(src));
		}
		else if (prop->IsA<FUInt64Property>())
		{
			dest.WriteUInt64(*prop->ContainerPtrToValuePtr<uint64>(src));
		}
		else if (prop->IsA<FInt8Property>())
		{
			dest.WriteInt8(*prop->ContainerPtrToValuePtr<int8>(src));
		}
		else if (prop->IsA<FInt16Property>())
		{
			dest.WriteInt16(*prop->ContainerPtrToValuePtr<int16>(src));
		}
		else if (prop->IsA<FIntProperty>())
		{
			dest.WriteInt32(*prop->ContainerPtrToValuePtr<int32>(src));
		}
		else if (prop->IsA<FInt64Property>())
		{
			dest.WriteInt64(*prop->ContainerPtrToValuePtr<int64>(src));
		}
		else if (prop->IsA<FFloatProperty>())
		{
			dest.WriteFloat(*prop->ContainerPtrToValuePtr<float>(src));
		}
		else if (prop->IsA<FDoubleProperty>())
		{
			dest.WriteDouble(*prop->ContainerPtrToValuePtr<double>(src));
		}
	};
	
	if (const auto numericProp = CastField<FNumericProperty>(UnderlyingProperty))
	{
		writeNumber(numericProp);
	}
	else if (UnderlyingProperty->IsA<FBoolProperty>())
	{
		dest.WriteBool(*UnderlyingProperty->ContainerPtrToValuePtr<bool>(src));
	}
	else if (const auto enumProp = CastField<FEnumProperty>(UnderlyingProperty))
	{
		writeNumber(enumProp->GetUnderlyingProperty());
	}
	else
	{
		checkNoEntry();
	}
}

void ZSharp::FZPrimitivePropertyVisitor::SetValue_InContainer(void* dest, const FZCallBufferSlot& src) const
{
	const auto readNumber = [dest, src](const FNumericProperty* prop)
	{
		if (prop->IsA<FByteProperty>())
		{
			const uint8 value = src.ReadUInt8();
			prop->SetValue_InContainer(dest, &value);
		}
		else if (prop->IsA<FUInt16Property>())
		{
			const uint16 value = src.ReadUInt16();
			prop->SetValue_InContainer(dest, &value);
		}
		else if (prop->IsA<FUInt32Property>())
		{
			const uint32 value = src.ReadUInt32();
			prop->SetValue_InContainer(dest, &value);
		}
		else if (prop->IsA<FUInt64Property>())
		{
			const uint64 value = src.ReadUInt64();
			prop->SetValue_InContainer(dest, &value);
		}
		else if (prop->IsA<FInt8Property>())
		{
			const int8 value = src.ReadInt8();
			prop->SetValue_InContainer(dest, &value);
		}
		else if (prop->IsA<FInt16Property>())
		{
			const int16 value = src.ReadInt16();
			prop->SetValue_InContainer(dest, &value);
		}
		else if (prop->IsA<FIntProperty>())
		{
			const int32 value = src.ReadInt32();
			prop->SetValue_InContainer(dest, &value);
		}
		else if (prop->IsA<FInt64Property>())
		{
			const int64 value = src.ReadInt64();
			prop->SetValue_InContainer(dest, &value);
		}
		else if (prop->IsA<FFloatProperty>())
		{
			const float value = src.ReadFloat();
			prop->SetValue_InContainer(dest, &value);
		}
		else if (prop->IsA<FDoubleProperty>())
		{
			const double value = src.ReadDouble();
			prop->SetValue_InContainer(dest, &value);
		}
	};
	
	if (const auto numericProp = CastField<FNumericProperty>(UnderlyingProperty))
	{
		readNumber(numericProp);
	}
	else if (UnderlyingProperty->IsA<FBoolProperty>())
	{
		const bool value = src.ReadBool();
		UnderlyingProperty->SetValue_InContainer(dest, &value);
	}
	else if (const auto enumProp = CastField<FEnumProperty>(UnderlyingProperty))
	{
		readNumber(enumProp->GetUnderlyingProperty());
	}
	else
	{
		checkNoEntry();
	}
}


