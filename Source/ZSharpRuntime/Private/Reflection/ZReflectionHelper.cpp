// Copyright Zero Games. All Rights Reserved.

#include "Reflection/ZReflectionHelper.h"

#include "ALC/ZRuntimeTypeUri.h"
#include "Trait/ZConjugateKey.h"
#include "UObject/PropertyOptional.h"

FString ZSharp::FZReflectionHelper::GetFieldConjugateKey(const UField* field)
{
	return field->GetPathName();
}

FString ZSharp::FZReflectionHelper::GetFieldClassConjugateKey(const FFieldClass* cls)
{
	static const TMap<FFieldClass*, FString> GProtoMap
	{
		{ FByteProperty::StaticClass(), TZConjugateKey<uint8>::Value },
		{ FUInt16Property::StaticClass(), TZConjugateKey<uint16>::Value },
		{ FUInt32Property::StaticClass(), TZConjugateKey<uint32>::Value },
		{ FUInt64Property::StaticClass(), TZConjugateKey<uint64>::Value },
		{ FInt8Property::StaticClass(), TZConjugateKey<int8>::Value },
		{ FInt16Property::StaticClass(), TZConjugateKey<int16>::Value },
		{ FIntProperty::StaticClass(), TZConjugateKey<int32>::Value },
		{ FInt64Property::StaticClass(), TZConjugateKey<int64>::Value },
		{ FFloatProperty::StaticClass(), TZConjugateKey<float>::Value },
		{ FDoubleProperty::StaticClass(), TZConjugateKey<double>::Value },
		{ FBoolProperty::StaticClass(), TZConjugateKey<bool>::Value },

		{ FStrProperty::StaticClass(), TZConjugateKey<FString>::Value },
		{ FUtf8StrProperty::StaticClass(), TZConjugateKey<FUtf8String>::Value },
		{ FAnsiStrProperty::StaticClass(), TZConjugateKey<FAnsiString>::Value },
		{ FNameProperty::StaticClass(), TZConjugateKey<FName>::Value },
		{ FTextProperty::StaticClass(), TZConjugateKey<FText>::Value },

		{ FArrayProperty::StaticClass(), TZConjugateKey<FZSelfDescriptiveScriptArray>::Value },
		{ FSetProperty::StaticClass(), TZConjugateKey<FZSelfDescriptiveScriptSet>::Value },
		{ FMapProperty::StaticClass(), TZConjugateKey<FZSelfDescriptiveScriptMap>::Value },
		{ FOptionalProperty::StaticClass(), TZConjugateKey<FZSelfDescriptiveOptional>::Value },

		{ FClassProperty::StaticClass(), TZConjugateKey<FZSelfDescriptiveSubclassOf>::Value },
		{ FSoftClassProperty::StaticClass(), TZConjugateKey<FZSelfDescriptiveSoftClassPtr>::Value },
		{ FSoftObjectProperty::StaticClass(), TZConjugateKey<FZSelfDescriptiveSoftObjectPtr>::Value },
		{ FWeakObjectProperty::StaticClass(), TZConjugateKey<FZSelfDescriptiveWeakObjectPtr>::Value },
		{ FLazyObjectProperty::StaticClass(), TZConjugateKey<FZSelfDescriptiveLazyObjectPtr>::Value },
		{ FInterfaceProperty::StaticClass(), TZConjugateKey<FZSelfDescriptiveScriptInterface>::Value },

		{ FFieldPathProperty::StaticClass(), TZConjugateKey<FFieldPath>::Value },
	};

	const FString* key = GProtoMap.Find(cls);
	return key ? *key : FString{};
}

ZSharp::FZRuntimeTypeUri ZSharp::FZReflectionHelper::GetContainerElementRuntimeTypeUriFromProperty(const FProperty* elementProperty)
{
	if (!ensure(!elementProperty->IsA<FArrayProperty>() &&
		!elementProperty->IsA<FSetProperty>() &&
		!elementProperty->IsA<FMapProperty>() &&
		!elementProperty->IsA<FOptionalProperty>() &&
		!elementProperty->IsA<FDelegateProperty>() &&
		!elementProperty->IsA<FMulticastDelegateProperty>()))
	{
		return {};
	}
	
	if (auto classProp = CastField<const FClassProperty>(elementProperty))
	{
		// Special cast for TObjectPtr<UClass> and UClass*.
		if (classProp->HasAllPropertyFlags(CPF_TObjectPtr) || !classProp->HasAnyPropertyFlags(CPF_UObjectWrapper))
		{
			return FZRuntimeTypeUri { GetFieldConjugateKey(classProp->PropertyClass) };
		}
	}
	else if (auto objectProp = CastField<const FObjectProperty>(elementProperty))
	{
		return FZRuntimeTypeUri { GetFieldConjugateKey(objectProp->PropertyClass) };
	}
	else if (auto structProp = CastField<const FStructProperty>(elementProperty))
	{
		return FZRuntimeTypeUri { GetFieldConjugateKey(structProp->Struct) };
	}
	else if (auto enumProp = CastField<const FEnumProperty>(elementProperty))
	{
		return FZRuntimeTypeUri { GetFieldConjugateKey(enumProp->GetEnum()) };
	}
	else if (auto delegateProp = CastField<const FDelegateProperty>(elementProperty))
	{
		return FZRuntimeTypeUri { GetFieldConjugateKey(delegateProp->SignatureFunction) };
	}
	else if (auto multicastInlineDelegateProp = CastField<const FMulticastInlineDelegateProperty>(elementProperty))
	{
		return FZRuntimeTypeUri { GetFieldConjugateKey(multicastInlineDelegateProp->SignatureFunction) };
	}
	else if (auto multicastSparseDelegateProp = CastField<const FMulticastSparseDelegateProperty>(elementProperty))
	{
		return FZRuntimeTypeUri { GetFieldConjugateKey(multicastSparseDelegateProp->SignatureFunction) };
	}
	else if (auto numericProp = CastField<const FNumericProperty>(elementProperty))
	{
		if (const UEnum* underlyingEnum = numericProp->GetIntPropertyEnum())
		{
			return FZRuntimeTypeUri { GetFieldConjugateKey(underlyingEnum) };
		}
	}

	FString rootKey = GetFieldClassConjugateKey(elementProperty->GetClass());
	if (rootKey.IsEmpty())
	{
		return {};
	}

	FZRuntimeTypeUri inner;
	if (auto classProp = CastField<const FClassProperty>(elementProperty))
	{
		// It must be TSubclassOf<T> at this point.
		inner = FZRuntimeTypeUri { GetFieldConjugateKey(classProp->MetaClass) };
	}
	else if (auto softClassProp = CastField<const FSoftClassProperty>(elementProperty))
	{
		inner = FZRuntimeTypeUri { GetFieldConjugateKey(softClassProp->MetaClass) };
	}
	else if (auto softObjectProperty = CastField<const FSoftObjectProperty>(elementProperty))
	{
		inner = FZRuntimeTypeUri { GetFieldConjugateKey(softObjectProperty->PropertyClass) };
	}
	else if (auto weakObjectProperty = CastField<const FWeakObjectProperty>(elementProperty))
	{
		inner = FZRuntimeTypeUri { GetFieldConjugateKey(weakObjectProperty->PropertyClass) };
	}
	else if (auto lazyObjectProperty = CastField<const FLazyObjectProperty>(elementProperty))
	{
		inner = FZRuntimeTypeUri { GetFieldConjugateKey(lazyObjectProperty->PropertyClass) };
	}
	else if (auto interfaceProperty = CastField<const FInterfaceProperty>(elementProperty))
	{
		inner = FZRuntimeTypeUri { GetFieldConjugateKey(interfaceProperty->InterfaceClass) };
	}

	return { rootKey, inner };
}

void ZSharp::FZReflectionHelper::ChangeEnumValueType(const FEnumProperty* srcProperty, const void* src, const FEnumProperty* destProperty, void* dest)
{
	FNumericProperty* srcUnderlyingProperty = srcProperty->GetUnderlyingProperty();
	FNumericProperty* destUnderlyingProperty = destProperty->GetUnderlyingProperty();

	if (srcUnderlyingProperty->GetClass() == destUnderlyingProperty->GetClass())
	{
		FMemory::Memcpy(dest, src, srcUnderlyingProperty->GetElementSize());
		return;
	}
	
	int64 srcValue = 0;
	if (auto uint8Prop = CastField<const FByteProperty>(srcUnderlyingProperty))
	{
		const uint8 value = uint8Prop->GetPropertyValue(src);
		srcValue = value;
	}
	else if (auto uint16Prop = CastField<const FUInt16Property>(srcUnderlyingProperty))
	{
		const uint16 value = uint16Prop->GetPropertyValue(src);
		srcValue = value;
	}
	else if (auto uint32Prop = CastField<const FUInt32Property>(srcUnderlyingProperty))
	{
		const uint32 value = uint32Prop->GetPropertyValue(src);
		srcValue = value;
	}
	else if (auto uint64Prop = CastField<const FUInt64Property>(srcUnderlyingProperty))
	{
		const uint64 value = uint64Prop->GetPropertyValue(src);
		check(value <= MAX_int64);
		srcValue = value;
	}
	else if (auto int8Prop = CastField<const FInt8Property>(srcUnderlyingProperty))
	{
		const int8 value = int8Prop->GetPropertyValue(src);
		srcValue = value;
	}
	else if (auto int16Prop = CastField<const FInt16Property>(srcUnderlyingProperty))
	{
		const int16 value = int16Prop->GetPropertyValue(src);
		srcValue = value;
	}
	else if (auto int32Prop = CastField<const FIntProperty>(srcUnderlyingProperty))
	{
		const int32 value = int32Prop->GetPropertyValue(src);
		srcValue = value;
	}
	else if (auto int64Prop = CastField<const FInt64Property>(srcUnderlyingProperty))
	{
		const int64 value = int64Prop->GetPropertyValue(src);
		srcValue = value;
	}
	else
	{
		checkNoEntry();
	}

	if (destUnderlyingProperty->IsA<FByteProperty>())
	{
		auto value = static_cast<const uint8>(srcValue);
		destUnderlyingProperty->CopySingleValue(dest, &value);
	}
	else if (destUnderlyingProperty->IsA<FUInt16Property>())
	{
		auto value = static_cast<const uint16>(srcValue);
		destUnderlyingProperty->CopySingleValue(dest, &value);
	}
	else if (destUnderlyingProperty->IsA<FUInt32Property>())
	{
		auto value = static_cast<const uint32>(srcValue);
		destUnderlyingProperty->CopySingleValue(dest, &value);
	}
	else if (destUnderlyingProperty->IsA<FUInt64Property>())
	{
		check(srcValue >= 0);
		auto value = static_cast<const uint64>(srcValue);
		destUnderlyingProperty->CopySingleValue(dest, &value);
	}
	else if (destUnderlyingProperty->IsA<FInt8Property>())
	{
		auto value = static_cast<const int8>(srcValue);
		destUnderlyingProperty->CopySingleValue(dest, &value);
	}
	else if (destUnderlyingProperty->IsA<FInt16Property>())
	{
		auto value = static_cast<const int16>(srcValue);
		destUnderlyingProperty->CopySingleValue(dest, &value);
	}
	else if (destUnderlyingProperty->IsA<FIntProperty>())
	{
		auto value = static_cast<const int32>(srcValue);
		destUnderlyingProperty->CopySingleValue(dest, &value);
	}
	else if (destUnderlyingProperty->IsA<FInt64Property>())
	{
		auto value = srcValue;
		destUnderlyingProperty->CopySingleValue(dest, &value);
	}
	else
	{
		checkNoEntry();
	}
}

bool ZSharp::FZReflectionHelper::IsFunctionBindableToDelegate(const UFunction* function, const UDelegateFunction* delegate)
{
	static uint64 GIgnoredFlags = UFunction::GetDefaultIgnoredSignatureCompatibilityFlags() | CPF_ParmFlags;
	
	if (!function || !delegate)
	{
		return false;
	}
	
	if (function == delegate)
	{
		// Delegate signature is not an invocable function.
		return false;
	}

	TFieldIterator<FProperty> itA(function);
	TFieldIterator<FProperty> itB(delegate);

	while (itA && (itA->PropertyFlags & CPF_Parm))
	{
		if (itB && (itB->PropertyFlags & CPF_Parm))
		{
			FProperty* propA = *itA;
			FProperty* propB = *itB;

			const uint64 PropertyMash = propA->PropertyFlags ^ propB->PropertyFlags;
			if ((PropertyMash & ~GIgnoredFlags) != 0)
			{
				// Flags mismatch between an argument of A and B
				return false;
			}

			// TODO: Allow covariance/contravariance
			if (!FStructUtils::ArePropertiesTheSame(propA, propB, false))
			{
				// Type mismatch between an argument of A and B
				return false;
			}
		}
		else
		{
			// B ran out of arguments before A did
			return false;
		}
		++itA;
		++itB;
	}

	// They matched all the way thru A's properties, but it could still be a mismatch if B has remaining parameters
	return !(itB && (itB->PropertyFlags & CPF_Parm));
}

bool ZSharp::FZReflectionHelper::IsPropertyForceCopy(const FProperty* property)
{
	if (property->GetOffset_ForInternal() > 0)
	{
		return false;
	}

	if (!property->GetOwnerVariant().IsA<UScriptStruct>())
	{
		return false;
	}
	
	if (property->IsA<FNumericProperty>() || property->IsA<FBoolProperty>() || property->IsA<FEnumProperty>())
	{
		return false;
	}

	if (property->IsA<FObjectProperty>())
	{
		// Special case for TSubclassOf<>.
		if (auto classProperty = CastField<const FClassProperty>(property))
		{
			if (classProperty->HasAllPropertyFlags(CPF_UObjectWrapper))
			{
				return true;
			}
		}

		return false;
	}
	
	return true;
}


