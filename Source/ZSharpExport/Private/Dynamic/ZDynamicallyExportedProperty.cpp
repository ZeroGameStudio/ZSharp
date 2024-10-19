// Copyright Zero Games. All Rights Reserved.


#include "Dynamic/ZDynamicallyExportedProperty.h"

#include "Reflection/ZReflectionHelper.h"
#include "Static/ZExportHelper.h"

ZSharp::FZDynamicallyExportedProperty* ZSharp::FZDynamicallyExportedProperty::Create(const FProperty* property, int32 index)
{
	auto exportedProperty = new FZDynamicallyExportedProperty { property, index };
	if (!exportedProperty->IsValid())
	{
		delete exportedProperty;
		exportedProperty = nullptr;
	}

	return exportedProperty;
}

FString ZSharp::FZDynamicallyExportedProperty::GetName() const
{
	FString name = FZReflectionHelper::GetFieldAliasedName(Property);
	if (Index)
	{
		name.AppendInt(Index);
	}
	
	return name;
}

FString ZSharp::FZDynamicallyExportedProperty::GetZCallName() const
{
	return FString::Printf(TEXT("up:/%s"), *Property->GetPathName());
}

ZSharp::FZFullyExportedTypeName ZSharp::FZDynamicallyExportedProperty::GetType() const
{
	FZFullyExportedTypeName name = FZExportHelper::GetFPropertyFullyExportedTypeName(Property);
	if (bNullInNotNullOut)
	{
		name.bNullable = true;
	}

	return name;
}

FString ZSharp::FZDynamicallyExportedProperty::GetUnderlyingType() const
{
	return FZExportHelper::GetUEnumFromProperty(Property) ? "int64" : FString{};
}

bool ZSharp::FZDynamicallyExportedProperty::IsNullInNotNullOut() const
{
	return bNullInNotNullOut;
}

ZSharp::EZExportedPropertyFlags ZSharp::FZDynamicallyExportedProperty::GetFlags() const
{
	return Flags;
}

int32 ZSharp::FZDynamicallyExportedProperty::GetIndex() const
{
	return Index;
}

ZSharp::FZDynamicallyExportedProperty::FZDynamicallyExportedProperty(const FProperty* property, int32 index)
	: Property(property)
	, bNullInNotNullOut(false)
	, Index(index)
	, Flags(EZExportedPropertyFlags::None)
{
	if (FZExportHelper::CanFPropertyBeNullInNotNullOut(property))
	{
		bNullInNotNullOut = true;
	}
	
	if (property->HasAllPropertyFlags(CPF_NativeAccessSpecifierPublic) || property->GetBoolMetaData("AllowPrivateAccess"))
	{
		Flags |= EZExportedPropertyFlags::Public;
	}
	else if (property->HasAllPropertyFlags(CPF_NativeAccessSpecifierProtected))
	{
		Flags |= EZExportedPropertyFlags::Protected;
	}
	else if (property->HasAllPropertyFlags(CPF_NativeAccessSpecifierPrivate))
	{
		Flags |= EZExportedPropertyFlags::Private;
	}

	Flags |= EZExportedPropertyFlags::Readable;
	if (!property->IsA<FMulticastDelegateProperty>())
	{
		Flags |= EZExportedPropertyFlags::Writable;
	}
}

bool ZSharp::FZDynamicallyExportedProperty::IsValid() const
{
	return GetType().IsValid();
}


