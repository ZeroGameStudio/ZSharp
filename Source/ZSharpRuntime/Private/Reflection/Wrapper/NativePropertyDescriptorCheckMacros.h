// Copyright Zero Games. All Rights Reserved.

#pragma once

#define CHECK_NATIVE_PROPERTY_DESCRIPTOR() \
if (const auto objectProperty = CastField<FObjectPropertyBase>(property)) \
{ \
	check(objectProperty->PropertyClass->IsNative()); \
	if (const auto classProperty = CastField<FClassProperty>(property)) \
	{ \
		check(classProperty->MetaClass->IsNative()); \
	} \
	else if (const auto softClassProperty = CastField<FSoftClassProperty>(property)) \
	{ \
		check(classProperty->MetaClass->IsNative()); \
	} \
} \
else if (const auto interfaceProperty = CastField<FInterfaceProperty>(property)) \
{ \
	check(interfaceProperty->InterfaceClass->IsNative()); \
} \
else if (const auto structProperty = CastField<FStructProperty>(property)) \
{ \
	check(structProperty->Struct->IsNative()); \
} \
else if (const auto enumProperty = CastField<FEnumProperty>(property)) \
{ \
	check(enumProperty->GetEnum()->IsNative()); \
} \
else if (const auto numericProperty = CastField<FNumericProperty>(property)) \
{ \
	const UEnum* enm = numericProperty->GetIntPropertyEnum(); \
	check(!enm || enm->IsNative()); \
} \
else if (const auto delegateProperty = CastField<FDelegateProperty>(property)) \
{ \
	check(delegateProperty->SignatureFunction->IsNative()); \
} \
else if (const auto multicastDelegateProperty = CastField<FMulticastDelegateProperty>(property)) \
{ \
	check(multicastDelegateProperty->SignatureFunction->IsNative()); \
}


