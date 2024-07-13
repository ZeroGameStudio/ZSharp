// Copyright Zero Games. All Rights Reserved.


#include "Static/ZExportHelper.h"

#include "UObject/PropertyOptional.h"
#include "Reflection/ZReflectionHelper.h"
#include "Trait/ZExportedTypeName.h"

FString ZSharp::FZExportHelper::GetUFieldExportKey(const UField* field)
{
	if (!field)
	{
		return {};
	}
	
	const FString name = FZReflectionHelper::GetUFieldAliasedName(field);
	const FString module = FZReflectionHelper::GetUFieldModuleName(field);
	return FString::Printf(TEXT("%s.%s"), *module, *name);
}

ZSharp::FZFullyExportedTypeName ZSharp::FZExportHelper::GetUFieldFullyExportedName(const UField* field)
{
	if (!field)
	{
		return {};
	}
	
	const FString assemblyName = FZReflectionHelper::GetUFieldAssemblyName(field);
	const FString moduleName = FZReflectionHelper::GetUFieldModuleName(field);
	if (!assemblyName.Len() || !moduleName.Len())
	{
		return {};
	}

	return { FString::Printf(TEXT("%s.%s"), *assemblyName, *moduleName), FZReflectionHelper::GetUFieldAliasedName(field), field->IsA<UClass>() };
}

ZSharp::FZFullyExportedTypeName ZSharp::FZExportHelper::GetFPropertyFullyExportedTypeName(const FProperty* property)
{
	static const TMap<FFieldClass*, FZFullyExportedTypeName> GProtoMap
	{
		{ FByteProperty::StaticClass(), TZExportedTypeName<uint8>::Get(), },
		{ FUInt16Property::StaticClass(), TZExportedTypeName<uint16>::Get(), },
		{ FUInt32Property::StaticClass(), TZExportedTypeName<uint32>::Get(), },
		{ FUInt64Property::StaticClass(), TZExportedTypeName<uint64>::Get(), },
		{ FInt8Property::StaticClass(), TZExportedTypeName<int8>::Get(), },
		{ FInt16Property::StaticClass(), TZExportedTypeName<int16>::Get(), },
		{ FIntProperty::StaticClass(), TZExportedTypeName<int32>::Get(), },
		{ FInt64Property::StaticClass(), TZExportedTypeName<int64>::Get(), },
		{ FFloatProperty::StaticClass(), TZExportedTypeName<float>::Get(), },
		{ FDoubleProperty::StaticClass(), TZExportedTypeName<double>::Get(), },
		{ FBoolProperty::StaticClass(), TZExportedTypeName<bool>::Get(), },

		{ FStrProperty::StaticClass(), TZExportedTypeName<FString>::Get(), },
		{ FNameProperty::StaticClass(), TZExportedTypeName<FName>::Get(), },
		{ FTextProperty::StaticClass(), TZExportedTypeName<FText>::Get(), },

		{ FFieldPathProperty::StaticClass(), TZExportedTypeName<FFieldPath>::Get(), },
	};
	
	TFunction<FZFullyExportedTypeName(const FProperty*, bool)> recurse;
	recurse = [&recurse](const FProperty* property, bool allowContainer)->FZFullyExportedTypeName
	{
		if (const FZFullyExportedTypeName* name = GProtoMap.Find(property->GetClass()))
		{
			return *name;
		}

		if (const auto classProp = CastField<FClassProperty>(property))
		{
			if (classProp->MetaClass == UObject::StaticClass())
			{
				return GetUFieldFullyExportedName(UClass::StaticClass());
			}
			
			FZFullyExportedTypeName name = TZExportedTypeName<FZSelfDescriptiveSubclassOf>::Get();
			name.Inner = GetUFieldFullyExportedName(classProp->MetaClass).ToSimple();
			return name;
		}
		else if (const auto objectProp = CastField<FObjectProperty>(property))
		{
			return GetUFieldFullyExportedName(objectProp->PropertyClass);
		}
		else if (const auto interfaceProp = CastField<FInterfaceProperty>(property))
		{
			FZFullyExportedTypeName name = TZExportedTypeName<FZSelfDescriptiveScriptInterface>::Get();
			name.Inner = GetUFieldFullyExportedName(interfaceProp->InterfaceClass).ToSimple();
			return name;
		}
		else if (const auto structProp = CastField<FStructProperty>(property))
		{
			return GetUFieldFullyExportedName(structProp->Struct);
		}
		else if (const auto enumProp = CastField<FEnumProperty>(property))
		{
			return GetUFieldFullyExportedName(enumProp->GetEnum());
		}
		else if (const auto softClassProp = CastField<FSoftClassProperty>(property))
		{
			FZFullyExportedTypeName name = TZExportedTypeName<FZSelfDescriptiveSoftClassPtr>::Get();
			name.Inner = GetUFieldFullyExportedName(softClassProp->MetaClass).ToSimple();
			return name;
		}
		else if (const auto softObjectProp = CastField<FSoftObjectProperty>(property))
		{
			FZFullyExportedTypeName name = TZExportedTypeName<FZSelfDescriptiveSoftObjectPtr>::Get();
			name.Inner = GetUFieldFullyExportedName(softObjectProp->PropertyClass).ToSimple();
			return name;
		}
		else if (const auto weakObjectProp = CastField<FWeakObjectProperty>(property))
		{
			FZFullyExportedTypeName name = TZExportedTypeName<FZSelfDescriptiveWeakObjectPtr>::Get();
			name.Inner = GetUFieldFullyExportedName(weakObjectProp->PropertyClass).ToSimple();
			return name;
		}
		else if (const auto lazyObjectProp = CastField<FLazyObjectProperty>(property))
		{
			FZFullyExportedTypeName name = TZExportedTypeName<FZSelfDescriptiveLazyObjectPtr>::Get();
			name.Inner = GetUFieldFullyExportedName(lazyObjectProp->PropertyClass).ToSimple();
			return name;
		}
		else if (const auto delegateProp = CastField<FDelegateProperty>(property))
		{
			FZFullyExportedTypeName name = TZExportedTypeName<FZSelfDescriptiveScriptDelegate>::Get();
			name.Inner = GetUFieldFullyExportedName(delegateProp->SignatureFunction).ToSimple();
			return name;
		}
		else if (const auto multicastDelegateProp = CastField<FMulticastDelegateProperty>(property))
		{
			FZFullyExportedTypeName name = TZExportedTypeName<FZSelfDescriptiveMulticastScriptDelegate>::Get();
			name.Inner = GetUFieldFullyExportedName(multicastDelegateProp->SignatureFunction).ToSimple();
			return name;
		}
		else if (const auto arrayProp = CastField<FArrayProperty>(property))
		{
			if (allowContainer)
			{
				FZFullyExportedTypeName name = TZExportedTypeName<FZSelfDescriptiveScriptArray>::Get();
				name.Inner = recurse(arrayProp->Inner, false).ToSimple();
				return name;
			}
		}
		else if (const auto setProp = CastField<FSetProperty>(property))
		{
			if (allowContainer)
			{
				FZFullyExportedTypeName name = TZExportedTypeName<FZSelfDescriptiveScriptArray>::Get();
				name.Inner = recurse(setProp->ElementProp, false).ToSimple();
				return name;
			}
		}
		else if (const auto mapProp = CastField<FMapProperty>(property))
		{
			if (allowContainer)
			{
				FZFullyExportedTypeName name = TZExportedTypeName<FZSelfDescriptiveScriptArray>::Get();
				name.Inner = recurse(mapProp->KeyProp, false).ToSimple();
				name.Outer = recurse(mapProp->ValueProp, false).ToSimple();
				return name;
			}
		}
		else if (const auto optionalProp = CastField<FOptionalProperty>(property))
		{
			if (allowContainer)
			{
				FZFullyExportedTypeName name = TZExportedTypeName<FZSelfDescriptiveScriptArray>::Get();
				name.Inner = recurse(optionalProp->GetValueProperty(), false).ToSimple();
				return name;
			}
		}

		return {};
	};

	return recurse(property, true);
}


