// Copyright Zero Games. All Rights Reserved.


#include "IZPropertyVisitor.h"

#include "ZFieldPathPropertyVisitor.h"
#include "ZSharpRuntimeLogChannels.h"
#include "Container/ZStructPropertyVisitor.h"
#include "Object/ZClassPropertyVisitor.h"
#include "Object/ZLazyObjectPropertyVisitor.h"
#include "Object/ZObjectPropertyVisitor.h"
#include "Object/ZSoftClassPropertyVisitor.h"
#include "Object/ZSoftObjectPropertyVisitor.h"
#include "Object/ZWeakObjectPropertyVisitor.h"
#include "Primitive/ZBoolPropertyVisitor.h"
#include "Primitive/ZEnumPropertyVisitor.h"
#include "Primitive/ZNumericPropertyVisitor.h"
#include "String/ZNamePropertyVisitor.h"
#include "String/ZStringPropertyVisitor.h"
#include "String/ZTextPropertyVisitor.h"

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
	else if (const auto fieldProp = CastField<FFieldPathProperty>(prop))
	{
		return MakeUnique<FZFieldPathPropertyVisitor>(fieldProp);
	}
	else if (const auto arrayProp = CastField<FArrayProperty>(prop))
	{

	}
	else if (const auto setProp = CastField<FSetProperty>(prop))
	{

	}
	else if (const auto mapProp = CastField<FMapProperty>(prop))
	{

	}
	else if (const auto optionalProp = CastField<FOptionalProperty>(prop))
	{
		
	}
	else if (const auto delegateProp = CastField<FDelegateProperty>(prop))
	{
		
	}
	else if (const auto multicastInlineDelegateProp = CastField<FMulticastInlineDelegateProperty>(prop))
	{
		
	}
	else if (const auto multicastSparseDelegateProp = CastField<FMulticastSparseDelegateProperty>(prop))
	{
		
	}

	UE_LOG(LogZSharpRuntime, Fatal, TEXT("Property type [%s] is not supported!!!"), *prop->GetClass()->GetName());
	return nullptr;
}


