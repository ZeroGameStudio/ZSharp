﻿// Copyright Zero Games. All Rights Reserved.


#include "Static/ZExportHelper.h"

#include "ZSharpExportSettings.h"
#include "UObject/PropertyOptional.h"
#include "Reflection/ZReflectionHelper.h"
#include "Trait/ZExportedTypeName.h"

FString ZSharp::FZExportHelper::GetUFieldExportKey(const UField* field)
{
	if (!field)
	{
		return {};
	}
	
	const FString name = FZReflectionHelper::GetFieldRedirectedFullName(field);
	const FString module = FZReflectionHelper::GetFieldModuleName(field);
	return FString::Printf(TEXT("%s.%s"), *module, *name);
}

ZSharp::FZFullyExportedTypeName ZSharp::FZExportHelper::GetUFieldFullyExportedName(const UField* field)
{
	if (!field)
	{
		return {};
	}
	
	const FString assemblyName = FZReflectionHelper::GetFieldAssemblyName(field);
	const FString moduleName = FZReflectionHelper::GetFieldModuleName(field);
	if (!assemblyName.Len() || !moduleName.Len())
	{
		return {};
	}

	return { FString::Printf(TEXT("%s.%s"), *assemblyName, *moduleName), FZReflectionHelper::GetFieldRedirectedFullName(field), field->IsA<UClass>() };
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
		if (const auto numericProp = CastField<FNumericProperty>(property))
		{
			if (const UEnum* underlyingEnum = numericProp->GetIntPropertyEnum())
			{
				return GetUFieldFullyExportedName(underlyingEnum);
			}
		}
		
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
			name.Inner.bNullable = false;
			return name.Inner.IsValid() ? name : FZFullyExportedTypeName{};
		}
		else if (const auto objectProp = CastField<FObjectProperty>(property))
		{
			return GetUFieldFullyExportedName(objectProp->PropertyClass);
		}
		else if (const auto interfaceProp = CastField<FInterfaceProperty>(property))
		{
			FZFullyExportedTypeName name = TZExportedTypeName<FZSelfDescriptiveScriptInterface>::Get();
			name.Inner = GetUFieldFullyExportedName(interfaceProp->InterfaceClass).ToSimple();
			name.Inner.bNullable = false;
			return name.Inner.IsValid() ? name : FZFullyExportedTypeName{};
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
			name.Inner.bNullable = false;
			return name.Inner.IsValid() ? name : FZFullyExportedTypeName{};
		}
		else if (const auto softObjectProp = CastField<FSoftObjectProperty>(property))
		{
			FZFullyExportedTypeName name = TZExportedTypeName<FZSelfDescriptiveSoftObjectPtr>::Get();
			name.Inner = GetUFieldFullyExportedName(softObjectProp->PropertyClass).ToSimple();
			name.Inner.bNullable = false;
			return name.Inner.IsValid() ? name : FZFullyExportedTypeName{};
		}
		else if (const auto weakObjectProp = CastField<FWeakObjectProperty>(property))
		{
			FZFullyExportedTypeName name = TZExportedTypeName<FZSelfDescriptiveWeakObjectPtr>::Get();
			name.Inner = GetUFieldFullyExportedName(weakObjectProp->PropertyClass).ToSimple();
			name.Inner.bNullable = false;
			return name.Inner.IsValid() ? name : FZFullyExportedTypeName{};
		}
		else if (const auto lazyObjectProp = CastField<FLazyObjectProperty>(property))
		{
			FZFullyExportedTypeName name = TZExportedTypeName<FZSelfDescriptiveLazyObjectPtr>::Get();
			name.Inner = GetUFieldFullyExportedName(lazyObjectProp->PropertyClass).ToSimple();
			name.Inner.bNullable = false;
			return name.Inner.IsValid() ? name : FZFullyExportedTypeName{};
		}
		else if (const auto delegateProp = CastField<FDelegateProperty>(property))
		{
			return GetUFieldFullyExportedName(delegateProp->SignatureFunction);
		}
		else if (const auto multicastInlineDelegateProp = CastField<FMulticastInlineDelegateProperty>(property))
		{
			return GetUFieldFullyExportedName(multicastInlineDelegateProp->SignatureFunction);
		}
		else if (const auto multicastSparseDelegateProp = CastField<FMulticastSparseDelegateProperty>(property))
		{
			return GetUFieldFullyExportedName(multicastSparseDelegateProp->SignatureFunction);
		}
		else if (const auto arrayProp = CastField<FArrayProperty>(property))
		{
			if (allowContainer)
			{
				FZFullyExportedTypeName name = TZExportedTypeName<FZSelfDescriptiveScriptArray>::Get();
				name.Inner = recurse(arrayProp->Inner, false).ToSimple();
				return name.Inner.IsValid() ? name : FZFullyExportedTypeName{};
			}
		}
		else if (const auto setProp = CastField<FSetProperty>(property))
		{
			if (allowContainer)
			{
				FZFullyExportedTypeName name = TZExportedTypeName<FZSelfDescriptiveScriptSet>::Get();
				name.Inner = recurse(setProp->ElementProp, false).ToSimple();
				return name.Inner.IsValid() ? name : FZFullyExportedTypeName{};
			}
		}
		else if (const auto mapProp = CastField<FMapProperty>(property))
		{
			if (allowContainer)
			{
				FZFullyExportedTypeName name = TZExportedTypeName<FZSelfDescriptiveScriptMap>::Get();
				name.Inner = recurse(mapProp->KeyProp, false).ToSimple();
				name.Outer = recurse(mapProp->ValueProp, false).ToSimple();
				return name.Inner.IsValid() && name.Outer.IsValid() ? name : FZFullyExportedTypeName{};
			}
		}
		else if (const auto optionalProp = CastField<FOptionalProperty>(property))
		{
			if (allowContainer)
			{
				FZFullyExportedTypeName name = TZExportedTypeName<FZSelfDescriptiveOptional>::Get();
				name.Inner = recurse(optionalProp->GetValueProperty(), false).ToSimple();
				return name.Inner.IsValid() ? name : FZFullyExportedTypeName{};
			}
		}

		return {};
	};

	return recurse(property, true);
}

const UEnum* ZSharp::FZExportHelper::GetUEnumFromProperty(const FProperty* property)
{
	if (const auto enumProperty = CastField<FEnumProperty>(property))
	{
		return enumProperty->GetEnum();
	}

	if (const auto numericProperty = CastField<FNumericProperty>(property))
	{
		return numericProperty->GetIntPropertyEnum();
	}

	return nullptr;
}

bool ZSharp::FZExportHelper::CanFPropertyBeNullInNotNullOut(const FProperty* property)
{
	// Null-in-not-null-out means it can be marked as nullable and not null at the same time
	// when used as property or ref parameter.

	// Primitives and enums is not nullable.
	if (property->IsA<FNumericProperty>() || property->IsA<FBoolProperty>() || property->IsA<FEnumProperty>())
	{
		return false;
	}
	
	// Object is nullable.
	if (property->IsA<FObjectProperty>())
	{
		return false;
	}

	// The rest are all null-in-not-null-out.
	return true;
}

bool ZSharp::FZExportHelper::IsNameDeprecated(const FString& name)
{
	FString upperName = name.ToUpper();
	return upperName.StartsWith("DEPRECATED_") || upperName.EndsWith("_DEPRECATED");
}

bool ZSharp::FZExportHelper::IsFieldDeprecated(FFieldVariant field)
{
	if (GetDefault<UZSharpExportSettings>()->ShouldTreatDeprecatedPostfixAsDeprecated())
	{
		if (IsNameDeprecated(field.GetName()))
		{
			return true;
		}
	}

	if (const auto cls = field.Get<UClass>())
	{
		if (cls->HasAllClassFlags(CLASS_Deprecated))
		{
			return true;
		}
	}

	if (const auto property = field.Get<FProperty>())
	{
		if (property->HasAllPropertyFlags(CPF_Deprecated))
		{
			return true;
		}
	}
	
	return false;
}

bool ZSharp::FZExportHelper::IsFieldEditorOnly(FFieldVariant field)
{
	if (const auto function = field.Get<UFunction>())
	{
		if (function->HasAllFunctionFlags(FUNC_EditorOnly))
		{
			return true;
		}
	}

	if (const auto property = field.Get<FProperty>())
	{
		if (property->HasAllPropertyFlags(CPF_EditorOnly))
		{
			return true;
		}
	}

	return false;
}

bool ZSharp::FZExportHelper::ShouldExportFieldBySettings(FFieldVariant field)
{
	const FString path = field.GetPathName();
	const UZSharpExportSettings* settings = GetDefault<UZSharpExportSettings>();
	if (settings->IsForceExportFieldPath(path))
	{
		return true;
	}

	if (settings->IsForceNotExportFieldPath(path))
	{
		return false;
	}
	
	const bool exportDeprecated = GetDefault<UZSharpExportSettings>()->ShouldExportDeprecatedFields();
	const bool exportEditorOnly = GetDefault<UZSharpExportSettings>()->ShouldExportEditorOnlyFields();

	if (!exportDeprecated && IsFieldDeprecated(field))
	{
		return false;
	}

	if (!exportEditorOnly && IsFieldEditorOnly(field))
	{
		return false;
	}

	return true;
}


