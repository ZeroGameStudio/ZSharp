// Copyright Zero Games. All Rights Reserved.


#include "Reflection/ZReflectionHelper.h"

#include "ZSharpRuntimeSettings.h"
#include "ALC/IZMasterAssemblyLoadContext.h"
#include "Trait/ZManagedTypeInfo.h"
#include "UObject/PropertyOptional.h"

namespace ZReflectionHelper_Private
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
				name.InsertAt(newWordIndex, "_");
			}
		}
		else
		{
			name.AppendInt(0);
		}
	}
}

FString ZSharp::FZReflectionHelper::GetFieldRedirectedName(FFieldVariant field)
{
	if (!field)
	{
		return {};
	}

	// Redirect.
	FString name = field.GetName();
	FString redirectedName = GetDefault<UZSharpRuntimeSettings>()->RedirectFieldName(field.GetPathName());
	if (!redirectedName.IsEmpty())
	{
		name = redirectedName;
	}

	// Apply rules for specific field type.
	if (const auto cls = field.Get<UClass>())
	{
		if (cls->HasAllClassFlags(CLASS_Interface))
		{
			name.InsertAt(0, 'I');
		}

		if (cls->HasAllClassFlags(CLASS_Deprecated) && !cls->GetName().ToUpper().EndsWith("_DEPRECATED"))
		{
			name.Append("_DEPRECATED");
		}
	}
	else if (const auto enm = field.Get<UEnum>())
	{
		if (!name.StartsWith("E"))
		{
			name.InsertAt(0, "E");
		}
	}
	else if (const auto delegate = field.Get<UDelegateFunction>())
	{
		static const FString GDelegatePostfix = "__DelegateSignature";
		if (delegate->HasAllFunctionFlags(FUNC_Delegate) && name.EndsWith(GDelegatePostfix))
		{
			name.LeftChopInline(GDelegatePostfix.Len());
		}
	}
	// IMPORTANT: This must be under delegate.
	else if (const auto function = field.Get<UFunction>())
	{
		if (function->HasAllFunctionFlags(FUNC_EditorOnly))
		{
			name.Append("_EDITORONLY");
		}
	}
	else if (const auto property = field.Get<FProperty>())
	{
		if (property->HasAllPropertyFlags(CPF_EditorOnly))
		{
			name.Append("_EDITORONLY");
		}

		if (property->HasAllPropertyFlags(CPF_Deprecated) && !property->GetName().ToUpper().EndsWith("_DEPRECATED"))
		{
			name.Append("_DEPRECATED");
		}
	}
	
	// Check conflict with owner if field is member property or member function.
	bool conflicts = false;
	if (field.IsA<UFunction>() && !field.IsA<UDelegateFunction>() || field.IsA<FProperty>())
	{
		// Owner should always exist.
		const auto owner = field.GetOwnerVariant().Get<UStruct>();
		TArray structsToCheck { owner };
		for (TFieldIterator<UDelegateFunction> it(owner); it; ++it)
		{
			if (*it != field.Get<UDelegateFunction>())
			{
				structsToCheck.Emplace(*it);
			}
		}
	
		for (const auto structToCheck : structsToCheck)
		{
			if (name == GetFieldRedirectedName(structToCheck))
			{
				conflicts = true;
				break;
			}
		}
	}

	// Finally check conflict with managed root class System.Object.
	if (!conflicts)
	{
		static const TSet<FString> GManagedConflicts
		{
			"GetType",
			"GetHashCode",
			"ToString",
			"MemberwiseClone",
			"Equals",
			"ReferenceEquals",
			"Finalize"
		};

		if (GManagedConflicts.Contains(name))
		{
			conflicts = true;
		}
	}

	if (conflicts)
	{
		ZReflectionHelper_Private::DeconflictName(name);
	}

	return name;
}

FString ZSharp::FZReflectionHelper::GetFieldRedirectedFullName(FFieldVariant field)
{
	FString name = GetFieldRedirectedName(field);
	if (const auto ownerField = field.GetOwnerVariant().Get<UField>())
	{
		name = GetFieldRedirectedFullName(ownerField).Append(".").Append(name);
	}

	return name;
}

FString ZSharp::FZReflectionHelper::GetFieldAssemblyName(FFieldVariant field)
{
	const FString moduleName = GetFieldModuleName(field);
	const FZModuleMappingContext* ctx = GetDefault<UZSharpRuntimeSettings>()->GetModuleMappingContext(moduleName);
	return ctx ? ctx->AssemblyName : "";
}

FString ZSharp::FZReflectionHelper::GetFieldModuleName(FFieldVariant field)
{
	if (!field)
	{
		return {};
	}
	
	FString res;
	const bool suc = field.GetOutermost()->GetName().Split("/", nullptr, &res, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	check(suc);
	return res;
}

bool ZSharp::FZReflectionHelper::IsFieldModuleMapped(FFieldVariant field)
{
	return !!GetDefault<UZSharpRuntimeSettings>()->GetModuleMappingContext(GetFieldModuleName(field));
}

const UField* ZSharp::FZReflectionHelper::GetUFieldClosestMappedAncestor(const UField* field)
{
	if (!field)
	{
		return nullptr;
	}
	
	if (IsFieldModuleMapped(field))
	{
		return field;
	}
	
	auto strct = Cast<UStruct>(field);
	if (strct)
	{
		for (const UStruct* current = strct->GetSuperStruct(); current; current = current->GetSuperStruct())
		{
			if (IsFieldModuleMapped(current))
			{
				return current;
			}
		}
	}

	return nullptr;
}

bool ZSharp::FZReflectionHelper::GetUFieldRuntimeTypeLocator(const UField* field, FZRuntimeTypeLocatorWrapper& outLocator)
{
	const UField* ancestor = GetUFieldClosestMappedAncestor(field);
	const FString moduleName = GetFieldModuleName(ancestor);
	if (!moduleName.Len())
	{
		return false;
	}
	
	outLocator.AssemblyName = GetFieldAssemblyName(ancestor);
	if (!outLocator.AssemblyName.Len())
	{
		return false;
	}

	FString name = GetFieldRedirectedFullName(ancestor);
	name.ReplaceCharInline('.', '+');
	outLocator.TypeName = FString::Printf(TEXT("%s.%s.%s"), *outLocator.AssemblyName, *moduleName, *name);

	return true;
}

bool ZSharp::FZReflectionHelper::GetFFieldClassRuntimeTypeLocator(const FFieldClass* cls, FZRuntimeTypeLocatorWrapper& outLocator)
{
	static const TMap<FFieldClass*, FZRuntimeTypeLocatorWrapper> GProtoMap
	{
		{ FByteProperty::StaticClass(), { TZManagedTypeInfo<uint8>::GetAssemblyName(), TZManagedTypeInfo<uint8>::GetFullName() } },
		{ FUInt16Property::StaticClass(), { TZManagedTypeInfo<uint16>::GetAssemblyName(), TZManagedTypeInfo<uint16>::GetFullName() } },
		{ FUInt32Property::StaticClass(), { TZManagedTypeInfo<uint32>::GetAssemblyName(), TZManagedTypeInfo<uint32>::GetFullName() } },
		{ FUInt64Property::StaticClass(), { TZManagedTypeInfo<uint64>::GetAssemblyName(), TZManagedTypeInfo<uint64>::GetFullName() } },
		{ FInt8Property::StaticClass(), { TZManagedTypeInfo<int8>::GetAssemblyName(), TZManagedTypeInfo<int8>::GetFullName() } },
		{ FInt16Property::StaticClass(), { TZManagedTypeInfo<int16>::GetAssemblyName(), TZManagedTypeInfo<int16>::GetFullName() } },
		{ FIntProperty::StaticClass(), { TZManagedTypeInfo<int32>::GetAssemblyName(), TZManagedTypeInfo<int32>::GetFullName() } },
		{ FInt64Property::StaticClass(), { TZManagedTypeInfo<int64>::GetAssemblyName(), TZManagedTypeInfo<int64>::GetFullName() } },
		{ FFloatProperty::StaticClass(), { TZManagedTypeInfo<float>::GetAssemblyName(), TZManagedTypeInfo<float>::GetFullName() } },
		{ FDoubleProperty::StaticClass(), { TZManagedTypeInfo<double>::GetAssemblyName(), TZManagedTypeInfo<double>::GetFullName() } },
		{ FBoolProperty::StaticClass(), { TZManagedTypeInfo<bool>::GetAssemblyName(), TZManagedTypeInfo<bool>::GetFullName() } },

		{ FStrProperty::StaticClass(), { TZManagedTypeInfo<FString>::GetAssemblyName(), TZManagedTypeInfo<FString>::GetFullName() } },
		{ FNameProperty::StaticClass(), { TZManagedTypeInfo<FName>::GetAssemblyName(), TZManagedTypeInfo<FName>::GetFullName() } },
		{ FTextProperty::StaticClass(), { TZManagedTypeInfo<FText>::GetAssemblyName(), TZManagedTypeInfo<FText>::GetFullName() } },

		{ FArrayProperty::StaticClass(), { TZManagedTypeInfo<FZSelfDescriptiveScriptArray>::GetAssemblyName(), TZManagedTypeInfo<FZSelfDescriptiveScriptArray>::GetFullName() } },
		{ FSetProperty::StaticClass(), { TZManagedTypeInfo<FZSelfDescriptiveScriptSet>::GetAssemblyName(), TZManagedTypeInfo<FZSelfDescriptiveScriptSet>::GetFullName() } },
		{ FMapProperty::StaticClass(), { TZManagedTypeInfo<FZSelfDescriptiveScriptMap>::GetAssemblyName(), TZManagedTypeInfo<FZSelfDescriptiveScriptMap>::GetFullName() } },
		{ FOptionalProperty::StaticClass(), { TZManagedTypeInfo<FZSelfDescriptiveOptional>::GetAssemblyName(), TZManagedTypeInfo<FZSelfDescriptiveOptional>::GetFullName() } },

		{ FClassProperty::StaticClass(), { TZManagedTypeInfo<FZSelfDescriptiveSubclassOf>::GetAssemblyName(), TZManagedTypeInfo<FZSelfDescriptiveSubclassOf>::GetFullName() } },
		{ FSoftClassProperty::StaticClass(), { TZManagedTypeInfo<FZSelfDescriptiveSoftClassPtr>::GetAssemblyName(), TZManagedTypeInfo<FZSelfDescriptiveSoftClassPtr>::GetFullName() } },
		{ FSoftObjectProperty::StaticClass(), { TZManagedTypeInfo<FZSelfDescriptiveSoftObjectPtr>::GetAssemblyName(), TZManagedTypeInfo<FZSelfDescriptiveSoftObjectPtr>::GetFullName() } },
		{ FWeakObjectProperty::StaticClass(), { TZManagedTypeInfo<FZSelfDescriptiveWeakObjectPtr>::GetAssemblyName(), TZManagedTypeInfo<FZSelfDescriptiveWeakObjectPtr>::GetFullName() } },
		{ FLazyObjectProperty::StaticClass(), { TZManagedTypeInfo<FZSelfDescriptiveLazyObjectPtr>::GetAssemblyName(), TZManagedTypeInfo<FZSelfDescriptiveLazyObjectPtr>::GetFullName() } },
		{ FInterfaceProperty::StaticClass(), { TZManagedTypeInfo<FZSelfDescriptiveScriptInterface>::GetAssemblyName(), TZManagedTypeInfo<FZSelfDescriptiveScriptInterface>::GetFullName() } },

		{ FFieldPathProperty::StaticClass(), { TZManagedTypeInfo<FFieldPath>::GetAssemblyName(), TZManagedTypeInfo<FFieldPath>::GetFullName() } },
	};

	const FZRuntimeTypeLocatorWrapper* proto = GProtoMap.Find(cls);
	if (!proto)
	{
		checkNoEntry()
		return false;
	}

	outLocator = *proto;
	return true;
}

bool ZSharp::FZReflectionHelper::GetNonContainerFPropertyRuntimeTypeLocator(const FProperty* property, FZRuntimeTypeLocatorWrapper& outLocator)
{
	if (!ensure(!property->IsA<FArrayProperty>() && !property->IsA<FSetProperty>() && !property->IsA<FMapProperty>() && !property->IsA<FOptionalProperty>()))
	{
		return false;
	}
	
	if (const auto classProp = CastField<FClassProperty>(property))
	{
		if (classProp->MetaClass == UObject::StaticClass())
		{
			return GetUFieldRuntimeTypeLocator(UClass::StaticClass(), outLocator);
		}
	}
	else if (const auto objectProp = CastField<FObjectProperty>(property))
	{
		return GetUFieldRuntimeTypeLocator(objectProp->PropertyClass, outLocator);
	}
	else if (const auto structProp = CastField<FStructProperty>(property))
	{
		return GetUFieldRuntimeTypeLocator(structProp->Struct, outLocator);
	}
	else if (const auto enumProp = CastField<FEnumProperty>(property))
	{
		return GetUFieldRuntimeTypeLocator(enumProp->GetEnum(), outLocator);
	}
	else if (const auto delegateProp = CastField<FDelegateProperty>(property))
	{
		return GetUFieldRuntimeTypeLocator(delegateProp->SignatureFunction, outLocator);
	}
	else if (const auto multicastInlineDelegateProp = CastField<FMulticastInlineDelegateProperty>(property))
	{
		return GetUFieldRuntimeTypeLocator(multicastInlineDelegateProp->SignatureFunction, outLocator);
	}
	else if (const auto multicastSparseDelegateProp = CastField<FMulticastSparseDelegateProperty>(property))
	{
		return GetUFieldRuntimeTypeLocator(multicastSparseDelegateProp->SignatureFunction, outLocator);
	}
	else if (const auto numericProp = CastField<FNumericProperty>(property))
	{
		if (const UEnum* underlyingEnum = numericProp->GetIntPropertyEnum())
		{
			return GetUFieldRuntimeTypeLocator(underlyingEnum, outLocator);
		}
	}
	
	if (!GetFFieldClassRuntimeTypeLocator(property->GetClass(), outLocator))
	{
		return false;
	}

	if (outLocator.TypeName.Contains("`"))
	{
		ensure(outLocator.TypeName.EndsWith("1"));
		ensure(property->IsA<FObjectPropertyBase>() && !property->IsA<FObjectProperty>());
		
		FZRuntimeTypeLocatorWrapper& inner = outLocator.TypeParameters.Emplace_GetRef();
		if (const auto classProp = CastField<FClassProperty>(property))
		{
			return GetUFieldRuntimeTypeLocator(classProp->MetaClass, inner);
		}
		else if (const auto softClassProp = CastField<FSoftClassProperty>(property))
		{
			return GetUFieldRuntimeTypeLocator(softClassProp->MetaClass, inner);
		}
		else if (const auto softObjectProperty = CastField<FSoftObjectProperty>(property))
		{
			return GetUFieldRuntimeTypeLocator(softObjectProperty->PropertyClass, inner);
		}
		else if (const auto weakObjectProperty = CastField<FWeakObjectProperty>(property))
		{
			return GetUFieldRuntimeTypeLocator(weakObjectProperty->PropertyClass, inner);
		}
		else if (const auto lazyObjectProperty = CastField<FLazyObjectProperty>(property))
		{
			return GetUFieldRuntimeTypeLocator(lazyObjectProperty->PropertyClass, inner);
		}
		else if (const auto interfaceProperty = CastField<FInterfaceProperty>(property))
		{
			return GetUFieldRuntimeTypeLocator(interfaceProperty->InterfaceClass, inner);
		}
		else
		{
			checkNoEntry();
		}
	}
	
	checkNoEntry();
	return false;
}

