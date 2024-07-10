// Copyright Zero Games. All Rights Reserved.


#include "Dynamic/ZDynamicallyExportedProperty.h"

#include "SoftClassPtr.h"
#include "ZSharpExportHelpers.h"
#include "Trait/ZExportedTypeName.h"

namespace ZSharp::ZDynamicallyExportedProperty_Private
{
	static bool GetExportedTypeName(const FProperty* property, FString& outName, bool allowContainer)
	{
		outName = {};
		
		if (property->IsA<FByteProperty>())
		{
			outName = TZExportedTypeName<uint8>::Get();
		}
		else if (property->IsA<FUInt16Property>())
		{
			outName = TZExportedTypeName<uint16>::Get();
		}
		else if (property->IsA<FUInt32Property>())
		{
			outName = TZExportedTypeName<uint32>::Get();
		}
		else if (property->IsA<FUInt64Property>())
		{
			outName = TZExportedTypeName<uint64>::Get();
		}
		else if (property->IsA<FInt8Property>())
		{
			outName = TZExportedTypeName<int8>::Get();
		}
		else if (property->IsA<FInt16Property>())
		{
			outName = TZExportedTypeName<int16>::Get();
		}
		else if (property->IsA<FIntProperty>())
		{
			outName = TZExportedTypeName<int32>::Get();
		}
		else if (property->IsA<FInt64Property>())
		{
			outName = TZExportedTypeName<int64>::Get();
		}
		else if (property->IsA<FFloatProperty>())
		{
			outName = TZExportedTypeName<float>::Get();
		}
		else if (property->IsA<FDoubleProperty>())
		{
			outName = TZExportedTypeName<double>::Get();
		}
		else if (property->IsA<FBoolProperty>())
		{
			outName = TZExportedTypeName<bool>::Get();
		}
		else if (property->IsA<FStrProperty>())
		{
			outName = TZExportedTypeName<FString>::Get();
		}
		else if (property->IsA<FNameProperty>())
		{
			outName = TZExportedTypeName<FName>::Get();
		}
		else if (property->IsA<FTextProperty>())
		{
			outName = TZExportedTypeName<FText>::Get();
		}
		else if (const auto softClassProp = CastField<FSoftClassProperty>(property))
		{
			if (softClassProp->MetaClass == UObject::StaticClass())
			{
				outName = TZExportedTypeName<FSoftClassPtr>::Get();
			}

			if (FZSharpExportHelpers::IsUFieldModuleMapped(softClassProp->PropertyClass))
			{
				outName = FString::Printf(TEXT("%s<%s>"), *TZExportedTypeName<FSoftClassPtr>::Get(),
				                       *FZSharpExportHelpers::GetUFieldAliasedName(softClassProp->MetaClass));
			}
		}
		else if (const auto softObjectProp = CastField<FSoftObjectProperty>(property))
		{
			if (softObjectProp->PropertyClass == UObject::StaticClass())
			{
				outName = TZExportedTypeName<FSoftObjectPtr>::Get();
			}

			if (FZSharpExportHelpers::IsUFieldModuleMapped(softObjectProp->PropertyClass))
			{
				outName = FString::Printf(TEXT("%s<%s>"), *TZExportedTypeName<FSoftObjectPtr>::Get(),
				                       *FZSharpExportHelpers::GetUFieldAliasedName(softObjectProp->PropertyClass));
			}
		}
		else if (const auto weakObjectProp = CastField<FWeakObjectProperty>(property))
		{
			if (weakObjectProp->PropertyClass == UObject::StaticClass())
			{
				outName = TZExportedTypeName<FWeakObjectPtr>::Get();
			}

			if (FZSharpExportHelpers::IsUFieldModuleMapped(weakObjectProp->PropertyClass))
			{
				outName = FString::Printf(TEXT("%s<%s>"), *TZExportedTypeName<FWeakObjectPtr>::Get(),
				                       *FZSharpExportHelpers::GetUFieldAliasedName(weakObjectProp->PropertyClass));
			}
		}
		else if (const auto lazyObjectProp = CastField<FLazyObjectProperty>(property))
		{
			if (lazyObjectProp->PropertyClass == UObject::StaticClass())
			{
				outName = TZExportedTypeName<FLazyObjectPtr>::Get();
			}

			if (FZSharpExportHelpers::IsUFieldModuleMapped(lazyObjectProp->PropertyClass))
			{
				outName = FString::Printf(TEXT("%s<%s>"), *TZExportedTypeName<FLazyObjectPtr>::Get(),
				                       *FZSharpExportHelpers::GetUFieldAliasedName(lazyObjectProp->PropertyClass));
			}
		}
		else if (const auto classProp = CastField<FClassProperty>(property))
		{
			if (classProp->MetaClass == UObject::StaticClass())
			{
				outName = FZSharpExportHelpers::GetUFieldAliasedName(classProp->PropertyClass).Append("?");
			}

			if (FZSharpExportHelpers::IsUFieldModuleMapped(classProp->MetaClass))
			{
				outName = FZSharpExportHelpers::GetUFieldAliasedName(classProp->PropertyClass).Append("?"); // @TODO
			}
		}
		else if (const auto objectProp = CastField<FObjectProperty>(property))
		{
			if (FZSharpExportHelpers::IsUFieldModuleMapped(objectProp->PropertyClass))
			{
				outName = FZSharpExportHelpers::GetUFieldAliasedName(objectProp->PropertyClass).Append("?");
			}
		}
		else if (const auto interfaceProp = CastField<FInterfaceProperty>(property))
		{
			if (interfaceProp->InterfaceClass == UInterface::StaticClass())
			{
				outName = TZExportedTypeName<FScriptInterface>::Get();
			}

			if (FZSharpExportHelpers::IsUFieldModuleMapped(interfaceProp->InterfaceClass))
			{
				outName = FString::Printf(TEXT("%s<%s>"), *TZExportedTypeName<FScriptInterface>::Get(),
				                       *FZSharpExportHelpers::GetUFieldAliasedName(interfaceProp->InterfaceClass));
			}
		}
		else if (const auto structProp = CastField<FStructProperty>(property))
		{
			if (FZSharpExportHelpers::IsUFieldModuleMapped(structProp->Struct))
			{
				outName = FZSharpExportHelpers::GetUFieldAliasedName(structProp->Struct);
			}
		}
		else if (const auto enumProp = CastField<FEnumProperty>(property))
		{
			if (FZSharpExportHelpers::IsUFieldModuleMapped(enumProp->GetEnum()))
			{
				outName = FZSharpExportHelpers::GetUFieldAliasedName(enumProp->GetEnum());
			}
		}
		else if (const auto arrayProp = CastField<FArrayProperty>(property))
		{
			if (allowContainer)
			{
				FString innerName;
				if (GetExportedTypeName(arrayProp->Inner, innerName, false))
				{
					outName = FString::Printf(TEXT("%s<%s>"), TEXT("UnrealArray") /* @FIXME */,
										   *innerName);
				}
			}
		}

		return !!outName.Len();
	}
}

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
	FString type;
	ZDynamicallyExportedProperty_Private::GetExportedTypeName(Property, type, true);
	return type;
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


