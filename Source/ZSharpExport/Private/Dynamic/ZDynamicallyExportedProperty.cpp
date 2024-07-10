// Copyright Zero Games. All Rights Reserved.


#include "Dynamic/ZDynamicallyExportedProperty.h"

#include "SoftClassPtr.h"
#include "ZSharpExportHelpers.h"
#include "Trait/ZExportedTypeName.h"

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
	if (name == FZSharpExportHelpers::GetUFieldAliasedName(Property->GetOwnerStruct()))
	{
		name.InsertAt(0, "__");
	}

	return name;
}

FString ZSharp::FZDynamicallyExportedProperty::GetZCallName() const
{
	return FString::Printf(TEXT("up:/%s"), *Property->GetPathName());
}

FString ZSharp::FZDynamicallyExportedProperty::GetType() const
{
	if (Property->IsA<FByteProperty>())
	{
		return TZExportedTypeName<uint8>::Get();
	}
	else if (Property->IsA<FUInt16Property>())
	{
		return TZExportedTypeName<uint16>::Get();
	}
	else if (Property->IsA<FUInt32Property>())
    {
    	return TZExportedTypeName<uint32>::Get();
    }
	else if (Property->IsA<FUInt64Property>())
	{
		return TZExportedTypeName<uint64>::Get();
	}
	else if (Property->IsA<FInt8Property>())
	{
		return TZExportedTypeName<int8>::Get();
	}
	else if (Property->IsA<FInt16Property>())
	{
		return TZExportedTypeName<int16>::Get();
	}
	else if (Property->IsA<FIntProperty>())
	{
		return TZExportedTypeName<int32>::Get();
	}
	else if (Property->IsA<FInt64Property>())
	{
		return TZExportedTypeName<int64>::Get();
	}
	else if (Property->IsA<FFloatProperty>())
	{
		return TZExportedTypeName<float>::Get();
	}
	else if (Property->IsA<FDoubleProperty>())
	{
		return TZExportedTypeName<double>::Get();
	}
	else if (Property->IsA<FBoolProperty>())
	{
		return TZExportedTypeName<bool>::Get();
	}
	else if (Property->IsA<FStrProperty>())
	{
		return TZExportedTypeName<FString>::Get();
	}
	else if (Property->IsA<FNameProperty>())
	{
		return TZExportedTypeName<FName>::Get();
	}
	else if (Property->IsA<FTextProperty>())
    {
		return TZExportedTypeName<FText>::Get();
    }
	else if (const auto softClassProp = CastField<FSoftClassProperty>(Property))
	{
		if (softClassProp->MetaClass == UObject::StaticClass())
		{
			return TZExportedTypeName<FSoftClassPtr>::Get();
		}

		if (FZSharpExportHelpers::IsUFieldModuleMapped(softClassProp->PropertyClass))
		{
			return FString::Printf(TEXT("%s<%s>"), *TZExportedTypeName<FSoftClassPtr>::Get(), *FZSharpExportHelpers::GetUFieldAliasedName(softClassProp->MetaClass));
		}
	}
	else if (const auto softObjectProp = CastField<FSoftObjectProperty>(Property))
	{
		if (softObjectProp->PropertyClass == UObject::StaticClass())
		{
			return TZExportedTypeName<FSoftObjectPtr>::Get();
		}

		if (FZSharpExportHelpers::IsUFieldModuleMapped(softObjectProp->PropertyClass))
		{
			return FString::Printf(TEXT("%s<%s>"), *TZExportedTypeName<FSoftObjectPtr>::Get(), *FZSharpExportHelpers::GetUFieldAliasedName(softObjectProp->PropertyClass));
		}
	}
	else if (const auto weakObjectProp = CastField<FWeakObjectProperty>(Property))
	{
		if (weakObjectProp->PropertyClass == UObject::StaticClass())
		{
			return TZExportedTypeName<FWeakObjectPtr>::Get();
		}

		if (FZSharpExportHelpers::IsUFieldModuleMapped(weakObjectProp->PropertyClass))
		{
			return FString::Printf(TEXT("%s<%s>"), *TZExportedTypeName<FWeakObjectPtr>::Get(), *FZSharpExportHelpers::GetUFieldAliasedName(weakObjectProp->PropertyClass));
		}
	}
	else if (const auto lazyObjectProp = CastField<FLazyObjectProperty>(Property))
	{
		if (lazyObjectProp->PropertyClass == UObject::StaticClass())
		{
			return TZExportedTypeName<FLazyObjectPtr>::Get();
		}

		if (FZSharpExportHelpers::IsUFieldModuleMapped(lazyObjectProp->PropertyClass))
		{
			return FString::Printf(TEXT("%s<%s>"), *TZExportedTypeName<FLazyObjectPtr>::Get(), *FZSharpExportHelpers::GetUFieldAliasedName(lazyObjectProp->PropertyClass));
		}
	}
	else if (const auto classProp = CastField<FClassProperty>(Property))
	{
		if (classProp->MetaClass == UObject::StaticClass())
		{
			return FZSharpExportHelpers::GetUFieldAliasedName(classProp->PropertyClass).Append("?");
		}

		if (FZSharpExportHelpers::IsUFieldModuleMapped(classProp->MetaClass))
		{
			return FZSharpExportHelpers::GetUFieldAliasedName(classProp->PropertyClass).Append("?"); // @TODO
		}
	}
	else if (const auto objectProp = CastField<FObjectProperty>(Property))
	{
		if (FZSharpExportHelpers::IsUFieldModuleMapped(objectProp->PropertyClass))
		{
			return FZSharpExportHelpers::GetUFieldAliasedName(objectProp->PropertyClass).Append("?");
		}
	}
	else if (const auto structProp = CastField<FStructProperty>(Property))
	{
		if (FZSharpExportHelpers::IsUFieldModuleMapped(structProp->Struct))
		{
			return FZSharpExportHelpers::GetUFieldAliasedName(structProp->Struct);
		}
	}
	else if (const auto enumProp = CastField<FEnumProperty>(Property))
	{
		if (FZSharpExportHelpers::IsUFieldModuleMapped(enumProp->GetEnum()))
		{
			return FZSharpExportHelpers::GetUFieldAliasedName(enumProp->GetEnum());
		}
	}

	//checkNoEntry();
	return {};
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
	return !!GetType().Len();
}



