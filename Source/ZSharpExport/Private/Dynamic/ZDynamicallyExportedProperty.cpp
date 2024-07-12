// Copyright Zero Games. All Rights Reserved.


#include "Dynamic/ZDynamicallyExportedProperty.h"

#include "Reflection/ZReflectionHelper.h"
#include "Static/ZExportHelper.h"

ZSharp::FZDynamicallyExportedProperty* ZSharp::FZDynamicallyExportedProperty::Create(FProperty* property, int32 index)
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
	FString name = Property->GetName();
	if (Property->IsA<FBoolProperty>() && name.StartsWith("b", ESearchCase::CaseSensitive))
	{
		name.RightChopInline(1);
		if (FChar::IsDigit(name[0]))
		{
			name.InsertAt(0, "__");
		}
	}
	if (Index)
	{
		name.AppendInt(Index);
	}
	if (name == FZReflectionHelper::GetUFieldAliasedName(Property->GetOwnerStruct()))
	{
		name.InsertAt(0, "__");
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

ZSharp::EZExportedPropertyFlags ZSharp::FZDynamicallyExportedProperty::GetFlags() const
{
	return Flags;
}

int32 ZSharp::FZDynamicallyExportedProperty::GetIndex() const
{
	return Index;
}

ZSharp::FZDynamicallyExportedProperty::FZDynamicallyExportedProperty(FProperty* property, int32 index)
	: Property(property)
	, Index(index)
	, Flags(EZExportedPropertyFlags::None)
{
	Flags |= EZExportedPropertyFlags::Readable;
	if (!property->HasAnyPropertyFlags(CPF_BlueprintReadOnly))
	{
		Flags |= EZExportedPropertyFlags::Writable;
	}
	
	if (property->HasAnyPropertyFlags(CPF_NativeAccessSpecifierPublic) || property->GetBoolMetaData("AllowPrivateAccess"))
	{
		Flags |= EZExportedPropertyFlags::Public;
	}
	else if (property->HasAnyPropertyFlags(CPF_NativeAccessSpecifierProtected))
	{
		Flags |= EZExportedPropertyFlags::Protected;
	}
	else if (property->HasAnyPropertyFlags(CPF_NativeAccessSpecifierPrivate))
	{
		Flags |= EZExportedPropertyFlags::Private;
	}
}

bool ZSharp::FZDynamicallyExportedProperty::IsValid() const
{
	return GetType().IsValid();
}


