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
	return FZExportHelper::GetFPropertyFullyExportedTypeName(Property);
}

FString ZSharp::FZDynamicallyExportedProperty::GetUnderlyingType() const
{
	return FZExportHelper::GetUEnumFromProperty(Property) ? "int64" : FString{};
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
	, Index(index)
	, Flags(EZExportedPropertyFlags::None)
{
	if (Property->HasAllPropertyFlags(CPF_NativeAccessSpecifierPublic) || Property->GetBoolMetaData("AllowPrivateAccess"))
	{
		Flags |= EZExportedPropertyFlags::Public;
	}
	else if (Property->HasAllPropertyFlags(CPF_NativeAccessSpecifierProtected))
	{
		Flags |= EZExportedPropertyFlags::Protected;
	}
	else if (Property->HasAllPropertyFlags(CPF_NativeAccessSpecifierPrivate))
	{
		Flags |= EZExportedPropertyFlags::Private;
	}

	Flags |= EZExportedPropertyFlags::Readable;

	if (property->IsA<FMulticastDelegateProperty>())
	{
		Flags |= EZExportedPropertyFlags::Event;
	}
	else
	{
		Flags |= EZExportedPropertyFlags::Writable;
	}
}

bool ZSharp::FZDynamicallyExportedProperty::IsValid() const
{
	return GetType().IsValid();
}


