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
	if (auto numericProp = CastField<const FNumericProperty>(prop))
	{
		return MakeUnique<FZNumericPropertyVisitor>(numericProp);
	}
	else if (auto boolProp = CastField<const FBoolProperty>(prop))
	{
		return MakeUnique<FZBoolPropertyVisitor>(boolProp);
	}
	else if (auto enumProp = CastField<const FEnumProperty>(prop))
	{
		return MakeUnique<FZEnumPropertyVisitor>(enumProp);
	}
	else if (auto strProp = CastField<const FStrProperty>(prop))
	{
		return MakeUnique<FZStringPropertyVisitor>(strProp);
	}
	else if (auto utf8StrProp = CastField<const FUtf8StrProperty>(prop))
	{
		return MakeUnique<FZUtf8StringPropertyVisitor>(utf8StrProp);
	}
	else if (auto ansiStrProp = CastField<const FAnsiStrProperty>(prop))
	{
		return MakeUnique<FZAnsiStringPropertyVisitor>(ansiStrProp);
	}
	else if (auto nameProp = CastField<const FNameProperty>(prop))
	{
		return MakeUnique<FZNamePropertyVisitor>(nameProp);
	}
	else if (auto textProp = CastField<const FTextProperty>(prop))
	{
		return MakeUnique<FZTextPropertyVisitor>(textProp);
	}
	else if (prop->IsA<FObjectPropertyBase>())
	{
		if (auto objectProp = CastField<const FObjectProperty>(prop))
		{
			if (auto classProp = CastField<const FClassProperty>(prop))
			{
				return MakeUnique<FZClassPropertyVisitor>(classProp);
			}
			else
			{
				return MakeUnique<FZObjectPropertyVisitor>(objectProp);
			}
		}
		else if (auto softObjectProp = CastField<const FSoftObjectProperty>(prop))
		{
			if (auto softClassProp = CastField<const FSoftClassProperty>(prop))
			{
				return MakeUnique<FZSoftClassPropertyVisitor>(softClassProp);
			}
			else
			{
				return MakeUnique<FZSoftObjectPropertyVisitor>(softObjectProp);
			}
		}
		else if (auto weakObjectProp = CastField<const FWeakObjectProperty>(prop))
		{
			return MakeUnique<FZWeakObjectPropertyVisitor>(weakObjectProp);
		}
		else if (auto lazyObjectProp = CastField<const FLazyObjectProperty>(prop))
        {
			return MakeUnique<FZLazyObjectPropertyVisitor>(lazyObjectProp);
        }
	}
	else if (auto structProp = CastField<const FStructProperty>(prop))
	{
		return MakeUnique<FZStructPropertyVisitor>(structProp);
	}
	else if (auto interfaceProp = CastField<const FInterfaceProperty>(prop))
	{
		return MakeUnique<FZInterfacePropertyVisitor>(interfaceProp);
	}
	else if (auto fieldProp = CastField<const FFieldPathProperty>(prop))
	{
		return MakeUnique<FZFieldPathPropertyVisitor>(fieldProp);
	}
	else if (auto arrayProp = CastField<const FArrayProperty>(prop))
	{
		return MakeUnique<FZArrayPropertyVisitor>(arrayProp);
	}
	else if (auto setProp = CastField<const FSetProperty>(prop))
	{
		return MakeUnique<FZSetPropertyVisitor>(setProp);
	}
	else if (auto mapProp = CastField<const FMapProperty>(prop))
	{
		return MakeUnique<FZMapPropertyVisitor>(mapProp);
	}
	else if (auto optionalProp = CastField<const FOptionalProperty>(prop))
	{
		return MakeUnique<FZOptionalPropertyVisitor>(optionalProp);
	}
	else if (auto delegateProp = CastField<const FDelegateProperty>(prop))
	{
		return MakeUnique<FZDelegatePropertyVisitor>(delegateProp);
	}
	else if (auto multicastInlineDelegateProp = CastField<const FMulticastInlineDelegateProperty>(prop))
	{
		return MakeUnique<FZMulticastInlineDelegatePropertyVisitor>(multicastInlineDelegateProp);
	}
	else if (auto multicastSparseDelegateProp = CastField<const FMulticastSparseDelegateProperty>(prop))
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
	GetValue(ContainerPtrToValuePtr(src, index), dest);
}

void ZSharp::IZPropertyVisitor::GetRef_InContainer(const void* src, FZCallBufferSlot& dest, int32 index) const
{
	if (FZReflectionHelper::IsPropertyForceCopy(GetUnderlyingProperty()))
	{
		GetValue_InContainer(src, dest, index);
		return;
	}
	
	GetRef(ContainerPtrToValuePtr(src, index), dest);
}

void ZSharp::IZPropertyVisitor::SetValue_InContainer(void* dest, const FZCallBufferSlot& src, int32 index) const
{
	SetValue(ContainerPtrToValuePtr(dest, index), src);
}

void ZSharp::IZPropertyVisitor::GetPropertyValue_InContainer(const void* src, FZCallBufferSlot& dest, int32 index) const
{
	checkSlow(!GetUnderlyingProperty()->GetOwnerStruct()->IsA<UFunction>());

	if (FZReflectionHelper::IsPropertyForceCopy(GetUnderlyingProperty()))
	{
		GetValue_InContainer(src, dest, index);
		return;
	}
	
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

void ZSharp::IZPropertyVisitor::SetPropertyValue_InContainer(void* dest, const FZCallBufferSlot& src, int32 index) const
{
	checkSlow(!GetUnderlyingProperty()->GetOwnerStruct()->IsA<UFunction>());
	
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


