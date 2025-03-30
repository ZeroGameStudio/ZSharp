// Copyright Zero Games. All Rights Reserved.

#include "ZExportHelper.h"

#include "ZSharpExportSettings.h"
#include "UObject/PropertyOptional.h"
#include "Trait/ZExportedTypeName.h"

namespace ZExportHelper_Private
{
	/**
	 * Single word name -> Add 0.
	 * Multiple word name -> Insert _.
	 * Example:
	 * Equals -> Equals0
	 * GetType -> Get_Type
	 */
	static void DeconflictName(FString& name)
	{
		TArray<int32> newWordIndices;
		for (int32 i = name.Len() - 1; i > 0; --i)
		{
			if (FChar::IsUpper(name[i]))
			{
				newWordIndices.Emplace(i);
			}
		}

		if (!newWordIndices.IsEmpty())
		{
			for (const auto& newWordIndex : newWordIndices)
			{
				name.InsertAt(newWordIndex, '_');
			}
		}
		else
		{
			name.AppendInt(0);
		}
	}

	static void ToCamelCase(FString& name)
	{
		if (name.IsEmpty())
		{
			return;
		}

		int32 len = name.Len();
		for (int32 i = 0; i < len; ++i)
		{
			if (i == 0 || i < len - 1 && FChar::IsUpper(name[i + 1]))
			{
				name[i] = FChar::ToLower(name[i]);
			}
		}
	}
}

FString ZSharp::FZExportHelper::GetFieldRedirectedName(FFieldVariant field)
{
	if (!field)
	{
		return {};
	}

	// Redirect.
	FString name = field.GetName();
	FString redirectedName = GetDefault<UZSharpExportSettings>()->RedirectFieldName(field.GetPathName());
#if WITH_METADATA
	if (redirectedName.IsEmpty())
	{
		static const FName GExportNameRedirect = "ZSharpExportNameRedirect";
	
		if (field.IsUObject())
		{
			redirectedName = CastChecked<UField>(field.ToUObjectUnsafe())->GetMetaData(GExportNameRedirect);
		}
		else
		{
			redirectedName = field.ToFieldUnsafe()->GetMetaData(GExportNameRedirect);
		}
	}
#endif

	if (!redirectedName.IsEmpty())
	{
		name = redirectedName;
	}

	// Apply rules for specific field type.
	if (auto cls = field.Get<const UClass>())
	{
		if (cls->HasAllClassFlags(CLASS_Interface))
		{
			name.InsertAt(0, 'I');
		}

		if (cls->HasAllClassFlags(CLASS_Deprecated) && !cls->GetName().EndsWith("_DEPRECATED"))
		{
			name.Append("_DEPRECATED");
		}
	}
	else if (auto enm = field.Get<const UEnum>())
	{
		if (!name.StartsWith("E", ESearchCase::CaseSensitive))
		{
			name.InsertAt(0, 'E');
		}
	}
	else if (auto delegate = field.Get<const UDelegateFunction>())
	{
		static const FString GDelegatePostfix = "__DelegateSignature";
		if (delegate->HasAllFunctionFlags(FUNC_Delegate) && name.EndsWith(GDelegatePostfix, ESearchCase::CaseSensitive))
		{
			name.LeftChopInline(GDelegatePostfix.Len());
		}
	}
	// IMPORTANT: This must be under delegate.
	else if (auto function = field.Get<const UFunction>())
	{
		if (function->HasAllFunctionFlags(FUNC_EditorOnly))
		{
			name.Append("_EDITORONLY");
		}
	}
	else if (auto property = field.Get<const FProperty>())
	{
		if (property->HasAllPropertyFlags(CPF_EditorOnly))
		{
			name.Append("_EDITORONLY");
		}

		if (property->HasAllPropertyFlags(CPF_Deprecated) && !property->GetName().EndsWith("_DEPRECATED"))
		{
			name.Append("_DEPRECATED");
		}

		// Convert parameter name to camel case: Xxx -> xxx, bYyy -> yyy (Assume there is no conflict).
		if (property->HasAllPropertyFlags(CPF_Parm))
		{
			if (field.IsA<FBoolProperty>() && name[0] == 'b' && name.Len() > 1 && (!FChar::IsLower(name[1]) && !FChar::IsDigit(name[1])))
			{
				name.RemoveAt(0);
			}

			ZExportHelper_Private::ToCamelCase(name);
		}
	}

	// Check conflict with owner if field is member property or member function.
	bool conflicts = false;
	if (field.IsA<UFunction>() && !field.IsA<UDelegateFunction>() || field.IsA<FProperty>())
	{
		// Owner should always exist.
		auto owner = field.GetOwnerVariant().Get<const UStruct>();
		TArray structsToCheck { owner };
		for (TFieldIterator<UDelegateFunction> it(owner); it; ++it)
		{
			if (*it != field.Get<UDelegateFunction>())
			{
				structsToCheck.Emplace(*it);
			}
		}
	
		for (const auto& structToCheck : structsToCheck)
		{
			if (name.Equals(GetFieldRedirectedName(structToCheck), ESearchCase::CaseSensitive))
			{
				conflicts = true;
				break;
			}
		}
	}

	// Check conflict with managed root class System.Object.
	if (!conflicts)
	{
		static const TArray<FString> GManagedConflicts
		{
			"GetType",
			"GetHashCode",
			"ToString",
			"MemberwiseClone",
			"Equals",
			"ReferenceEquals",
			"Finalize",
		};

		if (GManagedConflicts.ContainsByPredicate([&name](const FString& elem){ return name.Equals(elem, ESearchCase::CaseSensitive); }))
		{
			conflicts = true;
		}
	}

	if (conflicts)
	{
		ZExportHelper_Private::DeconflictName(name);
	}
	else
	{
		// Check conflict with reserved keywords.
		static const TArray<FString> GReservedKeywords
		{
			"abstract", "as", "base", "bool", "break", "byte", "case", "catch", "char", "checked", "class", "const", "continue",
			"decimal", "default", "delegate", "do", "double", "else", "enum", "event", "explicit", "extern", "false", "finally", 
			"fixed", "float", "for", "foreach", "goto", "if", "implicit", "in", "int", "interface", "internal", "is", "lock",
			"long", "namespace", "new", "null", "object", "operator", "out", "override", "params", "private", "protected", 
			"public", "readonly", "ref", "return", "sbyte", "sealed", "short", "sizeof", "stackalloc", "static", "string", "struct",
			"switch", "this", "throw", "true", "try", "typeof", "uint", "ulong", "unchecked", "unsafe", "ushort", "using", "virtual",
			"void", "volatile", "while",
		};

		if (GReservedKeywords.ContainsByPredicate([&name](const FString& elem){ return name.Equals(elem, ESearchCase::CaseSensitive); }))
		{
			name.InsertAt(0, '@');
		}
	}

	return name;
}

FString ZSharp::FZExportHelper::GetFieldRedirectedFullName(FFieldVariant field)
{
	FString name = GetFieldRedirectedName(field);
	if (auto ownerField = field.GetOwnerVariant().Get<const UField>())
	{
		name = GetFieldRedirectedFullName(ownerField).Append(".").Append(name);
	}

	return name;
}

FString ZSharp::FZExportHelper::GetFieldModuleName(FFieldVariant field)
{
	if (!field)
	{
		return {};
	}
	
	FString res;
	const bool suc = field.GetOutermost()->GetName().Split("/", nullptr, &res, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	ensure(suc);
	return res;
}

FString ZSharp::FZExportHelper::GetFieldAssemblyName(FFieldVariant field)
{
	const FString moduleName = GetFieldModuleName(field);
	const FZModuleMappingContext* ctx = GetDefault<UZSharpExportSettings>()->GetModuleMappingContext(moduleName);
	return ctx ? ctx->AssemblyName : "";
}

bool ZSharp::FZExportHelper::ShouldExportField(FFieldVariant field)
{
	const FString path = field.GetPathName();
	const UZSharpExportSettings* settings = GetDefault<UZSharpExportSettings>();
	
	{ // First class non-export reasons regardless of anything else.
#if WITH_METADATA
		// Compiled-in no export.
		if (field.HasMetaData("ZSharpNoExport"))
		{
			return false;
		}
#endif

		// Force no-export by settings
		if (settings->IsForceNotExportFieldPath(path))
		{
			return false;
		}
	}

	{ // No explicit settings for non-export but CANNOT export for some reason.
		// Module unmapped so can't export.
		if (!IsFieldModuleMapped(field))
		{
			return false;
		}
		
		{ // This field has some intrinsic limitation so can't export.
			// Only native field is exportable.
			if (!field.IsNative())
			{
				return false;
			}

			// Emitted type is not exportable.
			if (field.HasMetaData("ZSharpEmittedField"))
			{
				return false;
			}
			
			// Override version of UFunction is not exportable.
			if (auto function = field.Get<const UFunction>())
			{
				if (function->GetSuperFunction())
				{
					return false;
				}
			}

			// WeakObject parameter is not exportable. (Engine bug)
			if (auto parameter = field.Get<const FWeakObjectProperty>())
			{
				if (parameter->HasAllPropertyFlags(CPF_Parm))
				{
					return false;
				}
			}
		}
	}

	{ // Export/Non-export by metadata.
		// Force export by settings regardless of metadata.
		if (settings->IsForceExportFieldPath(path))
		{
			return true;
		}

		// Skip export by Deprecated flag.
		if (!settings->ShouldExportDeprecatedFields() && IsFieldDeprecated(field))
		{
			return false;
		}

		// Skip export by Editor-Only flag.
		if (!settings->ShouldExportEditorOnlyFields() && IsFieldEditorOnly(field))
		{
			return false;
		}

		// Skip export by LazyObject flag.
		if (!settings->ShouldExportLazyObjectFields() && field.IsA<FLazyObjectProperty>())
		{
			return false;
		}
	}

	// Fallback to exportable.
	return true;
}

const UField* ZSharp::FZExportHelper::GetUFieldClosestExportedAncestor(const UField* field)
{
	if (!field)
	{
		return nullptr;
	}
	
	if (ShouldExportField(field))
	{
		return field;
	}
	
	auto strct = Cast<UStruct>(field);
	if (strct)
	{
		for (const UStruct* current = strct->GetSuperStruct(); current; current = current->GetSuperStruct())
		{
			if (ShouldExportField(current))
			{
				return current;
			}
		}
	}

	return nullptr;
}

ZSharp::FZFullyExportedTypeName ZSharp::FZExportHelper::GetFieldFullyExportedTypeName(FFieldVariant field)
{
	if (field.IsUObject())
	{
		return GetUFieldFullyExportedTypeName(Cast<UField>(field.ToUObjectUnsafe()));
	}
	else
	{
		return GetFPropertyFullyExportedTypeName(CastField<FProperty>(field.ToFieldUnsafe()));
	}
}

const UEnum* ZSharp::FZExportHelper::GetUEnumFromProperty(const FProperty* property)
{
	if (auto enumProperty = CastField<const FEnumProperty>(property))
	{
		return enumProperty->GetEnum();
	}

	if (auto numericProperty = CastField<const FNumericProperty>(property))
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
	if (auto classProperty = CastField<const FClassProperty>(property))
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

ZSharp::FZExportedDefaultValue ZSharp::FZExportHelper::GetParameterDefaultValue(const FProperty* parameter)
{
	ensure(parameter->HasAllPropertyFlags(CPF_Parm));
	
	auto function = Cast<const UFunction>(parameter->GetOwnerStruct());
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
		signature = FString::Printf(TEXT("%s.%s"), *GetFieldRedirectedName(enm), *value);
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
	else if (auto structParameter = CastField<const FStructProperty>(parameter))
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

bool ZSharp::FZExportHelper::IsNameDeprecated(const FString& name)
{
	FString upperName = name.ToUpper();
	return upperName.StartsWith("DEPRECATED_") || upperName.EndsWith("_DEPRECATED");
}

ZSharp::FZFullyExportedTypeName ZSharp::FZExportHelper::GetUFieldFullyExportedTypeName(const UField* field)
{
	if (!field)
	{
		return {};
	}
	
	const FString assemblyName = GetFieldAssemblyName(field);
	const FString moduleName = GetFieldModuleName(field);
	if (!assemblyName.Len() || !moduleName.Len())
	{
		return {};
	}

	return { FString::Printf(TEXT("%s.%s"), *assemblyName, *moduleName), GetFieldRedirectedFullName(field), field->IsA<UClass>() };
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
		{ FAnsiStrProperty::StaticClass(), TZExportedTypeName<FAnsiString>::Get(), },
		{ FUtf8StrProperty::StaticClass(), TZExportedTypeName<FUtf8String>::Get(), },
		{ FNameProperty::StaticClass(), TZExportedTypeName<FName>::Get(), },
		{ FTextProperty::StaticClass(), TZExportedTypeName<FText>::Get(), },

		{ FFieldPathProperty::StaticClass(), TZExportedTypeName<FFieldPath>::Get(), },
	};
	
	TFunction<FZFullyExportedTypeName(const FProperty*, bool)> recurse;
	recurse = [&recurse](const FProperty* property, bool allowContainer)->FZFullyExportedTypeName
	{
		if (auto numericProp = CastField<const FNumericProperty>(property))
		{
			if (const UEnum* underlyingEnum = numericProp->GetIntPropertyEnum())
			{
				return GetUFieldFullyExportedTypeName(underlyingEnum);
			}
		}
		
		if (const FZFullyExportedTypeName* name = GProtoMap.Find(property->GetClass()))
		{
			return *name;
		}

		if (auto classProp = CastField<const FClassProperty>(property))
		{
			// Special cast for TObjectPtr<UClass> and UClass*.
			if (classProp->HasAllPropertyFlags(CPF_TObjectPtr) || !classProp->HasAnyPropertyFlags(CPF_UObjectWrapper))
			{
				return GetUFieldFullyExportedTypeName(classProp->PropertyClass);
			}
			
			FZFullyExportedTypeName name = TZExportedTypeName<FZSelfDescriptiveSubclassOf>::Get();
			name.Inner = GetUFieldFullyExportedTypeName(classProp->MetaClass).ToSimple();
			name.Inner.bNullable = false;
			return name.Inner.IsValid() ? name : FZFullyExportedTypeName{};
		}
		else if (auto objectProp = CastField<const FObjectProperty>(property))
		{
			return GetUFieldFullyExportedTypeName(objectProp->PropertyClass);
		}
		else if (auto interfaceProp = CastField<const FInterfaceProperty>(property))
		{
			FZFullyExportedTypeName name = TZExportedTypeName<FZSelfDescriptiveScriptInterface>::Get();
			name.Inner = GetUFieldFullyExportedTypeName(interfaceProp->InterfaceClass).ToSimple();
			name.Inner.bNullable = false;
			return name.Inner.IsValid() ? name : FZFullyExportedTypeName{};
		}
		else if (auto structProp = CastField<const FStructProperty>(property))
		{
			return GetUFieldFullyExportedTypeName(structProp->Struct);
		}
		else if (auto enumProp = CastField<const FEnumProperty>(property))
		{
			return GetUFieldFullyExportedTypeName(enumProp->GetEnum());
		}
		else if (auto softClassProp = CastField<const FSoftClassProperty>(property))
		{
			FZFullyExportedTypeName name = TZExportedTypeName<FZSelfDescriptiveSoftClassPtr>::Get();
			name.Inner = GetUFieldFullyExportedTypeName(softClassProp->MetaClass).ToSimple();
			name.Inner.bNullable = false;
			return name.Inner.IsValid() ? name : FZFullyExportedTypeName{};
		}
		else if (auto softObjectProp = CastField<const FSoftObjectProperty>(property))
		{
			FZFullyExportedTypeName name = TZExportedTypeName<FZSelfDescriptiveSoftObjectPtr>::Get();
			name.Inner = GetUFieldFullyExportedTypeName(softObjectProp->PropertyClass).ToSimple();
			name.Inner.bNullable = false;
			return name.Inner.IsValid() ? name : FZFullyExportedTypeName{};
		}
		else if (auto weakObjectProp = CastField<const FWeakObjectProperty>(property))
		{
			FZFullyExportedTypeName name = TZExportedTypeName<FZSelfDescriptiveWeakObjectPtr>::Get();
			name.Inner = GetUFieldFullyExportedTypeName(weakObjectProp->PropertyClass).ToSimple();
			name.Inner.bNullable = false;
			return name.Inner.IsValid() ? name : FZFullyExportedTypeName{};
		}
		else if (auto lazyObjectProp = CastField<const FLazyObjectProperty>(property))
		{
			FZFullyExportedTypeName name = TZExportedTypeName<FZSelfDescriptiveLazyObjectPtr>::Get();
			name.Inner = GetUFieldFullyExportedTypeName(lazyObjectProp->PropertyClass).ToSimple();
			name.Inner.bNullable = false;
			return name.Inner.IsValid() ? name : FZFullyExportedTypeName{};
		}
		else if (auto delegateProp = CastField<const FDelegateProperty>(property))
		{
			return GetUFieldFullyExportedTypeName(delegateProp->SignatureFunction);
		}
		else if (auto multicastInlineDelegateProp = CastField<const FMulticastInlineDelegateProperty>(property))
		{
			return GetUFieldFullyExportedTypeName(multicastInlineDelegateProp->SignatureFunction);
		}
		else if (auto multicastSparseDelegateProp = CastField<const FMulticastSparseDelegateProperty>(property))
		{
			return GetUFieldFullyExportedTypeName(multicastSparseDelegateProp->SignatureFunction);
		}
		else if (auto arrayProp = CastField<const FArrayProperty>(property))
		{
			if (allowContainer)
			{
				FZFullyExportedTypeName name = TZExportedTypeName<FZSelfDescriptiveScriptArray>::Get();
				name.Inner = recurse(arrayProp->Inner, false).ToSimple();
				return name.Inner.IsValid() ? name : FZFullyExportedTypeName{};
			}
		}
		else if (auto setProp = CastField<const FSetProperty>(property))
		{
			if (allowContainer)
			{
				FZFullyExportedTypeName name = TZExportedTypeName<FZSelfDescriptiveScriptSet>::Get();
				name.Inner = recurse(setProp->ElementProp, false).ToSimple();
				return name.Inner.IsValid() ? name : FZFullyExportedTypeName{};
			}
		}
		else if (auto mapProp = CastField<const FMapProperty>(property))
		{
			if (allowContainer)
			{
				FZFullyExportedTypeName name = TZExportedTypeName<FZSelfDescriptiveScriptMap>::Get();
				name.Inner = recurse(mapProp->KeyProp, false).ToSimple();
				name.Outer = recurse(mapProp->ValueProp, false).ToSimple();
				return name.Inner.IsValid() && name.Outer.IsValid() ? name : FZFullyExportedTypeName{};
			}
		}
		else if (auto optionalProp = CastField<const FOptionalProperty>(property))
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

bool ZSharp::FZExportHelper::IsFieldModuleMapped(FFieldVariant field)
{
	return !!GetDefault<UZSharpExportSettings>()->GetModuleMappingContext(GetFieldModuleName(field));
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

	if (auto cls = field.Get<const UClass>())
	{
		if (cls->HasAllClassFlags(CLASS_Deprecated))
		{
			return true;
		}
	}

	if (auto property = field.Get<const FProperty>())
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
	if (auto function = field.Get<const UFunction>())
	{
		if (function->HasAllFunctionFlags(FUNC_EditorOnly))
		{
			return true;
		}
	}

	if (auto property = field.Get<const FProperty>())
	{
		if (property->HasAllPropertyFlags(CPF_EditorOnly))
		{
			return true;
		}
	}

	return false;
}


