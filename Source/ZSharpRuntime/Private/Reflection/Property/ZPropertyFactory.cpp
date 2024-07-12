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
	static void* const GNameTypeId = reinterpret_cast<void*>(13);
	static void* const GTextTypeId = reinterpret_cast<void*>(14);

	static void* const GWeakObjectPtrTypeId = reinterpret_cast<void*>(15);
	static void* const GSoftObjectPtrTypeId = reinterpret_cast<void*>(16);
	static void* const GLazyObjectPtrTypeId = reinterpret_cast<void*>(17);
	static void* const GSoftClassPtrTypeId = reinterpret_cast<void*>(18);
	static void* const GScriptInterfaceTypeId = reinterpret_cast<void*>(19);
	
	static void* const GFieldPathTypeId = reinterpret_cast<void*>(19);

	template <std::derived_from<FProperty> T>
	T* Create(EPropertyFlags flags)
	{
		auto prop = new T(nullptr, "", RF_Public | RF_MarkAsNative | RF_Transient);
		prop->SetPropertyFlags(flags);
		return prop;
	}

	static constexpr EPropertyFlags PrimitiveFlags = CPF_ZeroConstructor | CPF_IsPlainOldData | CPF_NoDestructor | CPF_HasGetValueTypeHash;

	static TMap<const void*, TFunction<FProperty*(const FZPropertyDesc&)>> FactoryMap
	{
		{ GUInt8TypeId, [](const FZPropertyDesc&){ return Create<FByteProperty>(PrimitiveFlags); } },
		{ GUInt16TypeId, [](const FZPropertyDesc&){ return Create<FUInt16Property>(PrimitiveFlags); } },
		{ GUInt32TypeId, [](const FZPropertyDesc&){ return Create<FUInt32Property>(PrimitiveFlags); } },
		{ GUInt64TypeId, [](const FZPropertyDesc&){ return Create<FUInt64Property>(PrimitiveFlags); } },
		{ GInt8TypeId, [](const FZPropertyDesc&){ return Create<FInt8Property>(PrimitiveFlags); } },
		{ GInt16TypeId, [](const FZPropertyDesc&){ return Create<FInt16Property>(PrimitiveFlags); } },
		{ GInt32TypeId, [](const FZPropertyDesc&){ return Create<FIntProperty>(PrimitiveFlags); } },
		{ GInt64TypeId, [](const FZPropertyDesc&){ return Create<FInt64Property>(PrimitiveFlags); } },
		{ GFloatTypeId, [](const FZPropertyDesc&){ return Create<FFloatProperty>(PrimitiveFlags); } },
		{ GDoubleTypeId, [](const FZPropertyDesc&){ return Create<FDoubleProperty>(PrimitiveFlags); } },
		{ GBoolTypeId, [](const FZPropertyDesc&){ return Create<FBoolProperty>(PrimitiveFlags); } },
		
		{ GStringTypeId, [](const FZPropertyDesc&){ return Create<FStrProperty>(CPF_ZeroConstructor | CPF_HasGetValueTypeHash); } },
		{ GNameTypeId, [](const FZPropertyDesc&){ return Create<FNameProperty>(PrimitiveFlags); } },
		{ GTextTypeId, [](const FZPropertyDesc&){ return Create<FTextProperty>(CPF_None); } },

		{ GWeakObjectPtrTypeId, [](const FZPropertyDesc&){ return Create<FWeakObjectProperty>(PrimitiveFlags | CPF_UObjectWrapper); } },
		{ GSoftObjectPtrTypeId, [](const FZPropertyDesc&){ return Create<FSoftObjectProperty>(CPF_UObjectWrapper | CPF_HasGetValueTypeHash); } },
		{ GLazyObjectPtrTypeId, [](const FZPropertyDesc&){ return Create<FLazyObjectProperty>(CPF_IsPlainOldData | CPF_NoDestructor | CPF_UObjectWrapper | CPF_HasGetValueTypeHash); } },
		{ GSoftClassPtrTypeId, [](const FZPropertyDesc&){ return Create<FSoftClassProperty>(CPF_UObjectWrapper | CPF_HasGetValueTypeHash); } },
		{ GScriptInterfaceTypeId, [](const FZPropertyDesc&){ return Create<FInterfaceProperty>(PrimitiveFlags | CPF_UObjectWrapper); } },

		{ GFieldPathTypeId, [](const FZPropertyDesc&){ return Create<FFieldPathProperty>(CPF_HasGetValueTypeHash); } },
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
		auto prop = PropertyFactory_Private::Create<FObjectProperty>(CPF_None);
		prop->PropertyClass = cls;
		return prop;
	}
	else if (auto scriptStruct = Cast<UScriptStruct>(field))
	{
		auto prop = PropertyFactory_Private::Create<FStructProperty>(CPF_None);
		prop->ElementSize = scriptStruct->GetStructureSize();
		prop->Struct = scriptStruct;
		return prop;
	}
	else if (auto enm = Cast<UEnum>(field))
	{
		auto prop = new FEnumProperty { enm };
		prop->SetEnum(enm);
	}

	checkNoEntry();
	return nullptr;
}


