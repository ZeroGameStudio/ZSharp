// Copyright Zero Games. All Rights Reserved.

#include "ZDynamicallyExportedProperty.h"

#include "ZExportHelper.h"
#include "Reflection/ZReflectionHelper.h"

ZSharp::FZDynamicallyExportedProperty* ZSharp::FZDynamicallyExportedProperty::Create(const FProperty* property, int32 index)
{
	if (!FZExportHelper::ShouldExportField(property))
	{
		return nullptr;
	}
	
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
	FString name = FZExportHelper::GetFieldRedirectedName(Property);
	if (Property->ArrayDim > 1)
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
	return FZExportHelper::GetFieldFullyExportedTypeName(Property);
}

FString ZSharp::FZDynamicallyExportedProperty::GetUnderlyingType() const
{
	return FZExportHelper::GetUEnumFromProperty(Property) ? "int64" : FString{};
}

bool ZSharp::FZDynamicallyExportedProperty::IsNullInNotNullOut() const
{
	return bNullInNotNullOut;
}

bool ZSharp::FZDynamicallyExportedProperty::HasBlackConjugate() const
{
	return FZExportHelper::DoesFPropertyHaveBlackConjugate(Property);
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
	if (!property->IsA<FDelegateProperty>() && !property->HasAnyPropertyFlags(CPF_BlueprintReadOnly))
	{
		Flags |= EZExportedPropertyFlags::Writable;
	}

	if (FZReflectionHelper::IsPropertyForceCopy(Property))
	{
		Flags |= EZExportedPropertyFlags::ForceCopy;
	}
}

bool ZSharp::FZDynamicallyExportedProperty::IsValid() const
{
	return GetType().IsValid();
}


