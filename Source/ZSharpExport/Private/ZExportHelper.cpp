// Copyright Zero Games. All Rights Reserved.


#include "ZExportHelper.h"

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

	// Subclassof is null-in-not-null-out.
	if (const auto classProperty = CastField<FClassProperty>(property))
	{
		if (classProperty->MetaClass != UObject::StaticClass())
		{
			return true;
		}
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

ZSharp::FZExportedDefaultValue ZSharp::FZExportHelper::GetParameterDefaultValue(const FProperty* parameter)
{
	ensure(parameter->HasAllPropertyFlags(CPF_Parm));
	
	const auto function = Cast<UFunction>(parameter->GetOwnerStruct());
	if (!ensure(function))
	{
		return {};
	}

	if (!ensure(!function->HasAnyFunctionFlags(FUNC_Delegate)))
	{
		return {};
	}

	const FString parameterName = parameter->GetName();
	const FString* defaultValue = nullptr;
	if (GetDefault<UZSharpExportSettings>()->ShouldUseLooseDefaultParameterName())
	{
		defaultValue = function->FindMetaData(*parameterName);
	}

	if (!defaultValue)
	{
		const FName cppDefaultValueKey { FString::Printf(TEXT("CPP_Default_%s"), *parameterName) };
		defaultValue = function->FindMetaData(cppDefaultValueKey);
	}

	if (!defaultValue)
	{
		return {};
	}
	
	FString defaultValueText = *defaultValue;
	
	FString signature;
	FString body;

	if (const UEnum* enm = GetUEnumFromProperty(parameter))
	{
		FString value;
		if (!defaultValueText.Split("::", nullptr, &value, ESearchCase::IgnoreCase, ESearchDir::FromEnd))
		{
			value = defaultValueText;
		}
		signature = FString::Printf(TEXT("%s.%s"), *FZReflectionHelper::GetFieldRedirectedName(enm), *value);
	}
	else if (parameter->IsA<FNumericProperty>() || parameter->IsA<FBoolProperty>())
	{
		signature = defaultValueText.ToLower();
		if (parameter->IsA<FFloatProperty>())
		{
			signature.AppendChar(TEXT('f'));
		}
	}
	else if (parameter->IsA<FStrProperty>()
			 || parameter->IsA<FAnsiStrProperty>()
			 || parameter->IsA<FUtf8StrProperty>()
			 || parameter->IsA<FNameProperty>())
	{
		signature = "null";
		body = FString::Printf(TEXT("@\"%s\""), *defaultValueText);
	}
	else if (parameter->IsA<FNameProperty>())
	{
		signature = "null";
		FName value;
		parameter->ImportText_Direct(*defaultValueText, &value, nullptr, PPF_None);
		body = FString::Printf(TEXT("@\"%s\""), *value.ToString());
	}
	else if (parameter->IsA<FTextProperty>())
	{
		signature = "null";
		FText value;
		parameter->ImportText_Direct(*defaultValueText, &value, nullptr, PPF_None);
		body = FString::Printf(TEXT("@\"%s\""), *value.ToString());
	}
	else if (parameter->IsA<FObjectPropertyBase>())
	{
		signature = "null";
	}
	else if (const auto structParameter = CastField<FStructProperty>(parameter))
	{
		signature = "null";
		// Migrates from [UhtStructDefaultValue].
		const UScriptStruct* scriptStruct = structParameter->Struct;
		if (scriptStruct == TBaseStructure<FVector>::Get())
		{
			FVector value;
			parameter->ImportText_Direct(*defaultValueText, &value, nullptr, PPF_None);
			body = FString::Printf(TEXT("new() { X = %.6f, Y = %.6f, Z = %.6f }"), value.X, value.Y, value.Z);
		}
		else if (scriptStruct == TBaseStructure<FRotator>::Get())
		{
			FRotator value;
			parameter->ImportText_Direct(*defaultValueText, &value, nullptr, PPF_None);
			body = FString::Printf(TEXT("new() { Pitch = %.6f, Yaw = %.6f, Roll = %.6f }"), value.Pitch, value.Yaw, value.Roll);
		}
		else if (scriptStruct == TBaseStructure<FVector2D>::Get())
		{
			FVector2D value;
			parameter->ImportText_Direct(*defaultValueText, &value, nullptr, PPF_None);
			body = FString::Printf(TEXT("new() { X = %.3f, Y = %.3f }"), value.X, value.Y);
		}
		else if (scriptStruct == TBaseStructure<FLinearColor>::Get())
		{
			FLinearColor value;
			parameter->ImportText_Direct(*defaultValueText, &value, nullptr, PPF_None);
			body = FString::Printf(TEXT("new() { R = %.6ff, G = %.6ff, B = %.6ff, A = %.6ff }"), value.R, value.G, value.B, value.A);
		}
		else if (scriptStruct == TBaseStructure<FColor>::Get())
		{
			FColor value;
			parameter->ImportText_Direct(*defaultValueText, &value, nullptr, PPF_None);
			body = FString::Printf(TEXT("new() { R = %d, G = %d, B = %d, A = %d }"), value.R, value.G, value.B, value.A);
		}
		else
		{
			body = "new()";
		}
	}
	else
	{
		ensureAlways(false);
	}
	
	// @FIXME: This is now C# expressions, but it was supposed to be language-agnostic.
	return { { signature }, { body } };
}


