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
	FString name = Property->GetName();
	// if (Property->IsA<FBoolProperty>())
	// {
	// 	if (name.StartsWith("b", ESearchCase::CaseSensitive))
	// 	{
	// 		name.RightChopInline(1);
	// 	}
	// 	name.InsertAt(0, "Is");
	// }
	if (Index)
	{
		name.AppendInt(Index);
	}
	
	const UStruct* owner = Property->GetOwnerStruct();
	TArray structsToCheck { owner };
	for (TFieldIterator<UFunction> it(owner, EFieldIteratorFlags::ExcludeSuper); it; ++it)
	{
		if (it->HasAllFunctionFlags(FUNC_Delegate))
		{
			structsToCheck.Emplace(*it);
		}
	}
	
	for (const auto structToCheck : structsToCheck)
	{
		FString nameToCheck;
		FString alias = FZReflectionHelper::GetUFieldAliasedName(structToCheck);
		if (!alias.Split(".", nullptr, &nameToCheck, ESearchCase::IgnoreCase, ESearchDir::FromEnd))
		{
			nameToCheck = alias;
		}
	
		if (name == nameToCheck)
		{
			name.AppendInt(0);
			break;
		}
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

ZSharp::FZDynamicallyExportedProperty::FZDynamicallyExportedProperty(const FProperty* property, int32 index)
	: Property(property)
	, Index(index)
	, Flags(EZExportedPropertyFlags::None)
{
	Flags |= EZExportedPropertyFlags::Readable;
	Flags |= EZExportedPropertyFlags::Writable;
	
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
}

bool ZSharp::FZDynamicallyExportedProperty::IsValid() const
{
	return GetType().IsValid();
}


