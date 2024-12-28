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

ZSharp::FZRuntimeTypeUri ZSharp::FZReflectionHelper::GetContainerElementRuntimeTypeUriFromProperty(const FProperty* property)
{
	if (!ensure(!property->IsA<FArrayProperty>() && !property->IsA<FSetProperty>() && !property->IsA<FMapProperty>() && !property->IsA<FOptionalProperty>()))
	{
		return {};
	}
	
	if (const auto classProp = CastField<FClassProperty>(property))
	{
		if (classProp->MetaClass == UObject::StaticClass())
		{
			return FZRuntimeTypeUri { GetFieldConjugateKey(UClass::StaticClass()) };
		}
	}
	else if (const auto objectProp = CastField<FObjectProperty>(property))
	{
		return FZRuntimeTypeUri { GetFieldConjugateKey(objectProp->PropertyClass) };
	}
	else if (const auto structProp = CastField<FStructProperty>(property))
	{
		return FZRuntimeTypeUri { GetFieldConjugateKey(structProp->Struct) };
	}
	else if (const auto enumProp = CastField<FEnumProperty>(property))
	{
		return FZRuntimeTypeUri { GetFieldConjugateKey(enumProp->GetEnum()) };
	}
	else if (const auto delegateProp = CastField<FDelegateProperty>(property))
	{
		return FZRuntimeTypeUri { GetFieldConjugateKey(delegateProp->SignatureFunction) };
	}
	else if (const auto multicastInlineDelegateProp = CastField<FMulticastInlineDelegateProperty>(property))
	{
		return FZRuntimeTypeUri { GetFieldConjugateKey(multicastInlineDelegateProp->SignatureFunction) };
	}
	else if (const auto multicastSparseDelegateProp = CastField<FMulticastSparseDelegateProperty>(property))
	{
		return FZRuntimeTypeUri { GetFieldConjugateKey(multicastSparseDelegateProp->SignatureFunction) };
	}
	else if (const auto numericProp = CastField<FNumericProperty>(property))
	{
		if (const UEnum* underlyingEnum = numericProp->GetIntPropertyEnum())
		{
			return FZRuntimeTypeUri { GetFieldConjugateKey(underlyingEnum) };
		}
	}

	FString rootKey = GetFieldClassConjugateKey(property->GetClass());
	if (rootKey.IsEmpty())
	{
		return {};
	}

	// Fill generic parameter, here it is ObjectWrapper, not Container.
	ensure(property->IsA<FObjectPropertyBase>() && !property->IsA<FObjectProperty>());
		
	FZRuntimeTypeUri inner;
	if (const auto classProp = CastField<FClassProperty>(property))
	{
		inner = FZRuntimeTypeUri { GetFieldConjugateKey(classProp->MetaClass) };
	}
	else if (const auto softClassProp = CastField<FSoftClassProperty>(property))
	{
		inner = FZRuntimeTypeUri { GetFieldConjugateKey(softClassProp->MetaClass) };
	}
	else if (const auto softObjectProperty = CastField<FSoftObjectProperty>(property))
	{
		inner = FZRuntimeTypeUri { GetFieldConjugateKey(softObjectProperty->PropertyClass) };
	}
	else if (const auto weakObjectProperty = CastField<FWeakObjectProperty>(property))
	{
		inner = FZRuntimeTypeUri { GetFieldConjugateKey(weakObjectProperty->PropertyClass) };
	}
	else if (const auto lazyObjectProperty = CastField<FLazyObjectProperty>(property))
	{
		inner = FZRuntimeTypeUri { GetFieldConjugateKey(lazyObjectProperty->PropertyClass) };
	}
	else if (const auto interfaceProperty = CastField<FInterfaceProperty>(property))
	{
		inner = FZRuntimeTypeUri { GetFieldConjugateKey(interfaceProperty->InterfaceClass) };
	}
	else
	{
		checkNoEntry();
	}

	return { rootKey, inner };
}


