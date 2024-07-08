// Copyright Zero Games. All Rights Reserved.


#include "IZPropertyVisitor.h"

#include "ZClassPropertyVisitor.h"
#include "ZLazyObjectPropertyVisitor.h"
#include "ZObjectPropertyVisitor.h"
#include "ZPrimitivePropertyVisitor.h"
#include "ZSharpRuntimeLogChannels.h"
#include "ZSoftClassPropertyVisitor.h"
#include "ZSoftObjectPropertyVisitor.h"
#include "ZStructPropertyVisitor.h"
#include "ZWeakObjectPropertyVisitor.h"

TUniquePtr<ZSharp::IZPropertyVisitor> ZSharp::IZPropertyVisitor::Create(const FProperty* prop)
{
	if (prop->IsA<FNumericProperty>() || prop->IsA<FBoolProperty>() || prop->IsA<FEnumProperty>())
	{
		return MakeUnique<FZPrimitivePropertyVisitor>(prop);
	}
	else if (const auto strProp = CastField<FStrProperty>(prop))
	{
		
	}
	else if (const auto nameProp = CastField<FNameProperty>(prop))
	{
		
	}
	else if (const auto textProp = CastField<FTextProperty>(prop))
	{
		
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
	else if (const auto arrayProp = CastField<FArrayProperty>(prop))
	{
		
	}
	else if (const auto setProp = CastField<FSetProperty>(prop))
	{
		
	}
	else if (const auto mapProp = CastField<FMapProperty>(prop))
	{
		
	}
	else if (const auto fieldProp = CastField<FFieldPathProperty>(prop))
	{
		
	}

	UE_LOG(LogZSharpRuntime, Fatal, TEXT("Property type [%s] is not supported!!!"), *prop->GetClass()->GetName());
	return nullptr;
}


