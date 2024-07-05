// Copyright Zero Games. All Rights Reserved.


#include "ZPropertyFactory.h"

namespace ZSharp::PropertyFactory_Private
{
	// NOTE: KEEP SYNC WITH IntrinsicTypeId.cs
	static const void* GUInt8TypeId = (void*)1;
	static const void* GUInt16TypeId = (void*)2;
	static const void* GUInt32TypeId = (void*)3;
	static const void* GUInt64TypeId = (void*)4;
	static const void* GInt8TypeId = (void*)5;
	static const void* GInt16TypeId = (void*)6;
	static const void* GInt32TypeId = (void*)7;
	static const void* GInt64TypeId = (void*)8;
	static const void* GFloatTypeId = (void*)9;
	static const void* GDoubleTypeId = (void*)10;
	static const void* GBoolTypeId = (void*)11;

	static const void* GStringTypeId = (void*)12;
	static const void* GNameTypeId = (void*)13;
	static const void* GTextTypeId = (void*)14;

	static const void* GWeakObjectPtrTypeId = (void*)15;
	static const void* GSoftObjectPtrTypeId = (void*)16;
	static const void* GLazyObjectPtrTypeId = (void*)17;
	static const void* GSoftClassPtrTypeId = (void*)18;
	static const void* GScriptInterfaceTypeId = (void*)19;

	static TMap<const void*, TFunction<FProperty*(const FZPropertyDesc&)>> FactoryMap
	{
		{ GUInt8TypeId, [](const FZPropertyDesc&){ return new FByteProperty(nullptr, "", RF_Transient); } },
		{ GUInt16TypeId, [](const FZPropertyDesc&){ return new FUInt16Property(nullptr, "", RF_Transient); } },
		{ GUInt32TypeId, [](const FZPropertyDesc&){ return new FUInt32Property(nullptr, "", RF_Transient); } },
		{ GUInt64TypeId, [](const FZPropertyDesc&){ return new FUInt64Property(nullptr, "", RF_Transient); } },
		{ GInt8TypeId, [](const FZPropertyDesc&){ return new FInt8Property(nullptr, "", RF_Transient); } },
		{ GInt16TypeId, [](const FZPropertyDesc&){ return new FInt16Property(nullptr, "", RF_Transient); } },
		{ GInt32TypeId, [](const FZPropertyDesc&){ return new FIntProperty(nullptr, "", RF_Transient); } },
		{ GInt64TypeId, [](const FZPropertyDesc&){ return new FInt64Property(nullptr, "", RF_Transient); } },
		{ GFloatTypeId, [](const FZPropertyDesc&){ return new FFloatProperty(nullptr, "", RF_Transient); } },
		{ GDoubleTypeId, [](const FZPropertyDesc&){ return new FDoubleProperty(nullptr, "", RF_Transient); } },
		{ GBoolTypeId, [](const FZPropertyDesc&){ return new FBoolProperty(nullptr, "", RF_Transient); } },
		
		{ GStringTypeId, [](const FZPropertyDesc&){ return new FStrProperty(nullptr, "", RF_Transient); } },
		{ GNameTypeId, [](const FZPropertyDesc&){ return new FNameProperty(nullptr, "", RF_Transient); } },
		{ GTextTypeId, [](const FZPropertyDesc&){ return new FTextProperty(nullptr, "", RF_Transient); } },
	};
}

FProperty* ZSharp::FZPropertyFactory::Create(const FZPropertyDesc& desc)
{
	if (const auto& func = PropertyFactory_Private::FactoryMap.Find(desc.Descriptor))
	{
		return (*func)(desc);
	}

	UStruct* strct = static_cast<UStruct*>(desc.Descriptor);
	if (auto cls = Cast<UClass>(strct))
	{
		auto prop = new FObjectProperty(nullptr, "", RF_Transient);
		prop->PropertyClass = cls;
		return prop;
	}
	else if (auto scriptStruct = Cast<UScriptStruct>(strct))
	{
		auto prop = new FStructProperty(nullptr, "", RF_Transient);
		prop->ElementSize = scriptStruct->GetStructureSize();
		prop->Struct = scriptStruct;
		return prop;
	}

	checkNoEntry();
	return nullptr;
}


