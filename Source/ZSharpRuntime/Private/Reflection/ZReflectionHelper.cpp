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
		if (classProp->MetaClass == UObject::StaticClass())
		{
			return FZRuntimeTypeUri { GetFieldConjugateKey(UClass::StaticClass()) };
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

	// Fill generic parameter, here it is ObjectWrapper, not Container.
	ensure(elementProperty->IsA<FObjectPropertyBase>() && !elementProperty->IsA<FObjectProperty>());
		
	FZRuntimeTypeUri inner;
	if (auto classProp = CastField<const FClassProperty>(elementProperty))
	{
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
	else
	{
		checkNoEntry();
	}

	return { rootKey, inner };
}


