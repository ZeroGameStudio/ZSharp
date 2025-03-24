// Copyright Zero Games. All Rights Reserved.

#include "ZPropertyFactory.h"

namespace ZSharp::PropertyFactory_Private
{
	// NOTE: KEEP SYNC WITH IntrinsicTypeId.cs
	static void* const GUInt8TypeId = reinterpret_cast<void*>(1);
	static void* const GUInt16TypeId = reinterpret_cast<void*>(2);
	static void* const GUInt32TypeId = reinterpret_cast<void*>(3);
	static void* const GUInt64TypeId = reinterpret_cast<void*>(4);
	static void* const GInt8TypeId = reinterpret_cast<void*>(5);
	static void* const GInt16TypeId = reinterpret_cast<void*>(6);
	static void* const GInt32TypeId = reinterpret_cast<void*>(7);
	static void* const GInt64TypeId = reinterpret_cast<void*>(8);
	static void* const GFloatTypeId = reinterpret_cast<void*>(9);
	static void* const GDoubleTypeId = reinterpret_cast<void*>(10);
	static void* const GBoolTypeId = reinterpret_cast<void*>(11);

	static void* const GStringTypeId = reinterpret_cast<void*>(12);
	static void* const GUtf8StringTypeId = reinterpret_cast<void*>(13);
	static void* const GAnsiStringTypeId = reinterpret_cast<void*>(14);
	static void* const GNameTypeId = reinterpret_cast<void*>(15);
	static void* const GTextTypeId = reinterpret_cast<void*>(16);

	static void* const GSubclassOfTypeId = reinterpret_cast<void*>(17);
	static void* const GSoftClassPtrTypeId = reinterpret_cast<void*>(18);
	static void* const GSoftObjectPtrTypeId = reinterpret_cast<void*>(19);
	static void* const GWeakObjectPtrTypeId = reinterpret_cast<void*>(20);
	static void* const GLazyObjectPtrTypeId = reinterpret_cast<void*>(21);
	static void* const GScriptInterfaceTypeId = reinterpret_cast<void*>(22);
	
	static void* const GFieldPathTypeId = reinterpret_cast<void*>(23);

	template <std::derived_from<FProperty> T>
	T* Create(TFunctionRef<void(T*)> initialize = [](T*){})
	{
		class : public FArchive
		{
			virtual FArchive& operator<<(FObjectPtr& Value) override { return *this; }
		} ar;
		
		auto prop = new T(nullptr, NAME_None, RF_Public | RF_MarkAsNative | RF_Transient);

		initialize(prop);

		prop->Link(ar);
		
		return prop;
	}

	static TMap<const void*, TFunction<FProperty*(const FZPropertyDesc&)>> FactoryMap
	{
		{ GUInt8TypeId, [](const FZPropertyDesc&){ return Create<FByteProperty>(); } },
		{ GUInt16TypeId, [](const FZPropertyDesc&){ return Create<FUInt16Property>(); } },
		{ GUInt32TypeId, [](const FZPropertyDesc&){ return Create<FUInt32Property>(); } },
		{ GUInt64TypeId, [](const FZPropertyDesc&){ return Create<FUInt64Property>(); } },
		{ GInt8TypeId, [](const FZPropertyDesc&){ return Create<FInt8Property>(); } },
		{ GInt16TypeId, [](const FZPropertyDesc&){ return Create<FInt16Property>(); } },
		{ GInt32TypeId, [](const FZPropertyDesc&){ return Create<FIntProperty>(); } },
		{ GInt64TypeId, [](const FZPropertyDesc&){ return Create<FInt64Property>(); } },
		{ GFloatTypeId, [](const FZPropertyDesc&){ return Create<FFloatProperty>(); } },
		{ GDoubleTypeId, [](const FZPropertyDesc&){ return Create<FDoubleProperty>(); } },
		{ GBoolTypeId, [](const FZPropertyDesc&){ return Create<FBoolProperty>(); } },
		
		{ GStringTypeId, [](const FZPropertyDesc&){ return Create<FStrProperty>(); } },
		{ GUtf8StringTypeId, [](const FZPropertyDesc&){ return Create<FUtf8StrProperty>(); } },
		{ GAnsiStringTypeId, [](const FZPropertyDesc&){ return Create<FAnsiStrProperty>(); } },
		{ GNameTypeId, [](const FZPropertyDesc&){ return Create<FNameProperty>(); } },
		{ GTextTypeId, [](const FZPropertyDesc&){ return Create<FTextProperty>(); } },

		{ GSubclassOfTypeId, [](const FZPropertyDesc& desc){ return Create<FClassProperty>([&](FClassProperty* prop){ prop->PropertyFlags |= CPF_UObjectWrapper; prop->PropertyClass = UClass::StaticClass(); prop->MetaClass = static_cast<UClass*>(desc.Metadata); }); } },
		{ GSoftClassPtrTypeId, [](const FZPropertyDesc& desc){ return Create<FSoftClassProperty>([&](FSoftClassProperty* prop){ prop->PropertyFlags |= CPF_UObjectWrapper; prop->PropertyClass = UClass::StaticClass(); prop->MetaClass = static_cast<UClass*>(desc.Metadata); }); } },
		{ GSoftObjectPtrTypeId, [](const FZPropertyDesc& desc){ return Create<FSoftObjectProperty>([&](FSoftObjectProperty* prop){ prop->PropertyFlags |= CPF_UObjectWrapper; prop->PropertyClass = static_cast<UClass*>(desc.Metadata); }); } },
		{ GWeakObjectPtrTypeId, [](const FZPropertyDesc& desc){ return Create<FWeakObjectProperty>([&](FWeakObjectProperty* prop){ prop->PropertyFlags |= CPF_UObjectWrapper; prop->PropertyClass = static_cast<UClass*>(desc.Metadata); }); } },
		{ GLazyObjectPtrTypeId, [](const FZPropertyDesc& desc){ return Create<FLazyObjectProperty>([&](FLazyObjectProperty* prop){ prop->PropertyFlags |= CPF_UObjectWrapper; prop->PropertyClass = static_cast<UClass*>(desc.Metadata); }); } },
		{ GScriptInterfaceTypeId, [](const FZPropertyDesc& desc){ return Create<FInterfaceProperty>([&](FInterfaceProperty* prop){ prop->PropertyFlags |= CPF_UObjectWrapper; prop->InterfaceClass = static_cast<UClass*>(desc.Metadata); }); } },

		{ GFieldPathTypeId, [](const FZPropertyDesc&){ return Create<FFieldPathProperty>(); } },
	};
}

FProperty* ZSharp::FZPropertyFactory::Create(const FZPropertyDesc& desc)
{
	if (const auto& func = PropertyFactory_Private::FactoryMap.Find(desc.Descriptor))
	{
		return (*func)(desc);
	}

	UField* field = static_cast<UField*>(desc.Descriptor);
	if (auto cls = Cast<UClass>(field))
	{
		// @FIXME: ClassProperty
		if (cls == UClass::StaticClass())
		{
			return PropertyFactory_Private::Create<FClassProperty>([&](FClassProperty* prop)
			{
				prop->PropertyClass = cls;
				prop->MetaClass = UObject::StaticClass();
			});
		}
		else
		{
			return PropertyFactory_Private::Create<FObjectProperty>([&](FObjectProperty* prop)
			{
				prop->PropertyClass = cls;
			});
		}
	}
	else if (auto scriptStruct = Cast<UScriptStruct>(field))
	{
		return PropertyFactory_Private::Create<FStructProperty>([&](FStructProperty* prop)
		{
			prop->Struct = scriptStruct;
		});
	}
	else if (auto enm = Cast<UEnum>(field))
	{
		return PropertyFactory_Private::Create<FEnumProperty>([&](FEnumProperty* prop)
		{
			auto underlyingProperty = new FInt64Property { prop, NAME_None, RF_NoFlags };
			prop->SetEnum(enm);
			prop->AddCppProperty(underlyingProperty);
		});
	}
	else if (auto sign = Cast<UDelegateFunction>(field); sign && !sign->IsA<USparseDelegateFunction>())
	{
		if (!sign->HasAnyFunctionFlags(FUNC_MulticastDelegate))
		{
			return PropertyFactory_Private::Create<FDelegateProperty>([&](FDelegateProperty* prop)
			{
				prop->SignatureFunction = sign;
			});
		}
		else
		{
			return PropertyFactory_Private::Create<FMulticastInlineDelegateProperty>([&](FMulticastInlineDelegateProperty* prop)
			{
				prop->SignatureFunction = sign;
			});
		}
	}

	checkNoEntry();
	return nullptr;
}


