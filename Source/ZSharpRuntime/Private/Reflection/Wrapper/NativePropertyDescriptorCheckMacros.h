// Copyright Zero Games. All Rights Reserved.

#pragma once

#define CHECK_NATIVE_PROPERTY_DESCRIPTOR() \
if (auto objectProperty = CastField<const FObjectPropertyBase>(property)) \
{ \
	check(objectProperty->PropertyClass->IsNative()); \
	if (auto classProperty = CastField<const FClassProperty>(property)) \
	{ \
		check(classProperty->MetaClass->IsNative()); \
	} \
	else if (auto softClassProperty = CastField<const FSoftClassProperty>(property)) \
	{ \
		check(softClassProperty->MetaClass->IsNative()); \
	} \
} \
else if (auto interfaceProperty = CastField<const FInterfaceProperty>(property)) \
{ \
	check(interfaceProperty->InterfaceClass->IsNative()); \
} \
else if (auto structProperty = CastField<const FStructProperty>(property)) \
{ \
	check(structProperty->Struct->IsNative()); \
} \
else if (auto enumProperty = CastField<const FEnumProperty>(property)) \
{ \
	check(enumProperty->GetEnum()->IsNative()); \
} \
else if (auto numericProperty = CastField<const FNumericProperty>(property)) \
{ \
	const UEnum* enm = numericProperty->GetIntPropertyEnum(); \
	check(!enm || enm->IsNative()); \
} \
else if (auto delegateProperty = CastField<const FDelegateProperty>(property)) \
{ \
	check(delegateProperty->SignatureFunction->IsNative()); \
} \
else if (auto multicastDelegateProperty = CastField<const FMulticastDelegateProperty>(property)) \
{ \
	check(multicastDelegateProperty->SignatureFunction->IsNative()); \
}


