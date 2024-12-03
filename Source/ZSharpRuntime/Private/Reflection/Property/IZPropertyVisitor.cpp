// Copyright Zero Games. All Rights Reserved.


#include "Reflection/Property/IZPropertyVisitor.h"

#include "UObject/PropertyOptional.h"
#include "ZSharpRuntimeLogChannels.h"
#include "ZFieldPathPropertyVisitor.h"
#include "Container/ZArrayPropertyVisitor.h"
#include "Container/ZMapPropertyVisitor.h"
#include "Container/ZOptionalPropertyVisitor.h"
#include "Container/ZSetPropertyVisitor.h"
#include "Container/ZStructPropertyVisitor.h"
#include "Delegate/ZDelegatePropertyVisitor.h"
#include "Delegate/ZMulticastInlineDelegatePropertyVisitor.h"
#include "Delegate/ZMulticastSparseDelegatePropertyVisitor.h"
#include "Object/ZObjectPropertyVisitor.h"
#include "Object/ZObjectWrapperPropertyVisitors.h"
#include "Primitive/ZBoolPropertyVisitor.h"
#include "Primitive/ZEnumPropertyVisitor.h"
#include "Primitive/ZNumericPropertyVisitor.h"
#include "String/ZAnsiStringPropertyVisitor.h"
#include "String/ZNamePropertyVisitor.h"
#include "String/ZStringPropertyVisitor.h"
#include "String/ZTextPropertyVisitor.h"
#include "String/ZUtf8StringPropertyVisitor.h"

TUniquePtr<ZSharp::IZPropertyVisitor> ZSharp::IZPropertyVisitor::Create(const FProperty* prop)
{
	if (const auto numericProp = CastField<FNumericProperty>(prop))
	{
		return MakeUnique<FZNumericPropertyVisitor>(numericProp);
	}
	else if (const auto boolProp = CastField<FBoolProperty>(prop))
	{
		return MakeUnique<FZBoolPropertyVisitor>(boolProp);
	}
	else if (const auto enumProp = CastField<FEnumProperty>(prop))
	{
		return MakeUnique<FZEnumPropertyVisitor>(enumProp);
	}
	else if (const auto strProp = CastField<FStrProperty>(prop))
	{
		return MakeUnique<FZStringPropertyVisitor>(strProp);
	}
	else if (const auto utf8StrProp = CastField<FUtf8StrProperty>(prop))
	{
		return MakeUnique<FZUtf8StringPropertyVisitor>(utf8StrProp);
	}
	else if (const auto ansiStrProp = CastField<FAnsiStrProperty>(prop))
	{
		return MakeUnique<FZAnsiStringPropertyVisitor>(ansiStrProp);
	}
	else if (const auto nameProp = CastField<FNameProperty>(prop))
	{
		return MakeUnique<FZNamePropertyVisitor>(nameProp);
	}
	else if (const auto textProp = CastField<FTextProperty>(prop))
	{
		return MakeUnique<FZTextPropertyVisitor>(textProp);
	}
	else if (prop->IsA<FObjectPropertyBase>())
	{
		if (const auto objectProp = CastField<FObjectProperty>(prop))
		{
			if (const auto classProp = CastField<FClassProperty>(prop))
			{
				return MakeUnique<FZClassPropertyVisitor>(classProp);
			}
			else
			{
				return MakeUnique<FZObjectPropertyVisitor>(objectProp);
			}
		}
		else if (const auto softObjectProp = CastField<FSoftObjectProperty>(prop))
		{
			if (const auto softClassProp = CastField<FSoftClassProperty>(prop))
			{
				return MakeUnique<FZSoftClassPropertyVisitor>(softClassProp);
			}
			else
			{
				return MakeUnique<FZSoftObjectPropertyVisitor>(softObjectProp);
			}
		}
		else if (const auto weakObjectProp = CastField<FWeakObjectProperty>(prop))
		{
			return MakeUnique<FZWeakObjectPropertyVisitor>(weakObjectProp);
		}
		else if (const auto lazyObjectProp = CastField<FLazyObjectProperty>(prop))
        {
			return MakeUnique<FZLazyObjectPropertyVisitor>(lazyObjectProp);
        }
	}
	else if (const auto structProp = CastField<FStructProperty>(prop))
	{
		return MakeUnique<FZStructPropertyVisitor>(structProp);
	}
	else if (const auto interfaceProp = CastField<FInterfaceProperty>(prop))
	{
		return MakeUnique<FZInterfacePropertyVisitor>(interfaceProp);
	}
	else if (const auto fieldProp = CastField<FFieldPathProperty>(prop))
	{
		return MakeUnique<FZFieldPathPropertyVisitor>(fieldProp);
	}
	else if (const auto arrayProp = CastField<FArrayProperty>(prop))
	{
		return MakeUnique<FZArrayPropertyVisitor>(arrayProp);
	}
	else if (const auto setProp = CastField<FSetProperty>(prop))
	{
		return MakeUnique<FZSetPropertyVisitor>(setProp);
	}
	else if (const auto mapProp = CastField<FMapProperty>(prop))
	{
		return MakeUnique<FZMapPropertyVisitor>(mapProp);
	}
	else if (const auto optionalProp = CastField<FOptionalProperty>(prop))
	{
		return MakeUnique<FZOptionalPropertyVisitor>(optionalProp);
	}
	else if (const auto delegateProp = CastField<FDelegateProperty>(prop))
	{
		return MakeUnique<FZDelegatePropertyVisitor>(delegateProp);
	}
	else if (const auto multicastInlineDelegateProp = CastField<FMulticastInlineDelegateProperty>(prop))
	{
		return MakeUnique<FZMulticastInlineDelegatePropertyVisitor>(multicastInlineDelegateProp);
	}
	else if (const auto multicastSparseDelegateProp = CastField<FMulticastSparseDelegateProperty>(prop))
	{
		return MakeUnique<FZMulticastSparseDelegatePropertyVisitor>(multicastSparseDelegateProp);
	}

	UE_LOG(LogZSharpRuntime, Fatal, TEXT("Property type [%s] is not supported!!!"), *prop->GetClass()->GetName());
	return nullptr;
}

void ZSharp::IZPropertyVisitor::InitializeValue_InContainer(void* dest) const
{
	InitializeValue(ContainerPtrToValuePtr(dest, 0));
}

void ZSharp::IZPropertyVisitor::DestructValue_InContainer(void* dest) const
{
	DestructValue(ContainerPtrToValuePtr(dest, 0));
}

void ZSharp::IZPropertyVisitor::GetValue_InContainer(const void* src, FZCallBufferSlot& dest, int32 index) const
{
	if (const FProperty* underlyingProperty = GetUnderlyingProperty(); underlyingProperty->HasGetter())
	{
		check(underlyingProperty->ArrayDim == 1);
		void* value = FMemory_Alloca(underlyingProperty->GetSize());
		InitializeValue(value);
		underlyingProperty->CallGetter(src, value);
		GetValue(value, dest);
	}
	else
	{
		GetValue(ContainerPtrToValuePtr(src, index), dest);
	}
}

void ZSharp::IZPropertyVisitor::GetRef_InContainer(const void* src, FZCallBufferSlot& dest, int32 index) const
{
	if (const FProperty* underlyingProperty = GetUnderlyingProperty(); underlyingProperty->HasGetter())
	{
		// We have no way but fallback to get a copy when get ref on a property with getter.
		check(underlyingProperty->ArrayDim == 1);
		void* value = FMemory_Alloca(underlyingProperty->GetSize());
		InitializeValue(value);
		underlyingProperty->CallGetter(src, value);
		GetValue(ContainerPtrToValuePtr(src, index), dest);
	}
	else
	{
		GetRef(ContainerPtrToValuePtr(src, index), dest);
	}
}

void ZSharp::IZPropertyVisitor::SetValue_InContainer(void* dest, const FZCallBufferSlot& src, int32 index) const
{
	if (const FProperty* underlyingProperty = GetUnderlyingProperty(); underlyingProperty->HasSetter())
	{
		check(underlyingProperty->ArrayDim == 1);
		void* value = FMemory_Alloca(underlyingProperty->GetSize());
		InitializeValue(value);
		SetValue(value, src);
		underlyingProperty->CallSetter(dest, value);
	}
	else
	{
		SetValue(ContainerPtrToValuePtr(dest, index), src);
	}
}


