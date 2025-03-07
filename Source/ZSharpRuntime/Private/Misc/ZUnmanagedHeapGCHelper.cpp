// Copyright Zero Games. All Rights Reserved.

#include "Misc/ZUnmanagedHeapGCHelper.h"

#include "UObject/FastReferenceCollector.h"
#include "UObject/PropertyOptional.h"

namespace ZSharp::ZUnmanagedHeapGCHelper_Private
{
	// Migrate from UObjectGlobals.cpp.
	static constexpr EClassCastFlags GWeakCastFlags =			CASTCLASS_FWeakObjectProperty | 
																CASTCLASS_FLazyObjectProperty |
																CASTCLASS_FSoftObjectProperty |
																CASTCLASS_FDelegateProperty |
																CASTCLASS_FMulticastDelegateProperty;

	static constexpr EClassCastFlags GUnreferencingCastFlags =	CASTCLASS_FByteProperty |
																CASTCLASS_FInt8Property |
																CASTCLASS_FIntProperty |
																CASTCLASS_FFloatProperty |
																CASTCLASS_FUInt64Property |
																CASTCLASS_FUInt32Property |
																CASTCLASS_FNameProperty |
																CASTCLASS_FStrProperty |
																CASTCLASS_FBoolProperty |
																CASTCLASS_FUInt16Property |
																CASTCLASS_FInt64Property |
																CASTCLASS_FNumericProperty |
																CASTCLASS_FTextProperty |
																CASTCLASS_FInt16Property |
																CASTCLASS_FDoubleProperty |
																CASTCLASS_FEnumProperty |
																CASTCLASS_FLargeWorldCoordinatesRealProperty;
	
	static bool MayContainStrongReference(const FProperty& property)
	{
		auto flags = static_cast<EClassCastFlags>(property.GetClass()->GetCastFlags());
		return !EnumHasAnyFlags(flags, GUnreferencingCastFlags | GWeakCastFlags);
	}
}

bool ZSharp::FZUnmanagedHeapGCHelper::CollectHeapReference(FReferenceCollector& collector, UObject*& reference)
{
	// Migrate from CollectStackReference() in UObjectGlobals.cpp.
	if (reference)
	{
		FReferenceCollector::AROPrivate::AddReferencedObject(collector, reference);
		return !reference;
	}
	
	return false;
}

void ZSharp::FZUnmanagedHeapGCHelper::CollectInterfaceReference(FReferenceCollector& collector, FScriptInterface& instance)
{
	// Migrate from CollectInterfaceReference() in UObjectGlobals.cpp.
	UObject*& reference = UE::Core::Private::Unsafe::Decay(instance.GetObjectRef());
	if (CollectHeapReference(collector, reference))
	{
		instance.SetInterface(nullptr);
	}
}

void ZSharp::FZUnmanagedHeapGCHelper::CollectFieldPathReference(FReferenceCollector& collector, FFieldPath& instance)
{
	// Migrate from CollectFieldPathReference() in UObjectGlobals.cpp.
	if (FUObjectItem* owner = FGCInternals::GetResolvedOwner(instance))
	{
		auto reference = static_cast<UObject*>(owner->Object);		
		if (CollectHeapReference(collector, reference))
		{
			FGCInternals::ClearCachedField(instance);
		}
	}
}

void ZSharp::FZUnmanagedHeapGCHelper::CollectPropertyReference(FReferenceCollector& collector, const FProperty& property, void* value)
{
	// Migrate from CollectPropertyReference() in UObjectGlobals.cpp.
	check(property.ArrayDim == 1);
	
	auto castFlags = static_cast<EClassCastFlags>(property.GetClass()->GetCastFlags());
	check(!EnumHasAnyFlags(castFlags, CASTCLASS_FArrayProperty | CASTCLASS_FSetProperty | CASTCLASS_FMapProperty | CASTCLASS_FOptionalProperty));
	if (EnumHasAnyFlags(castFlags, ZUnmanagedHeapGCHelper_Private::GWeakCastFlags))
	{
		return;
	}

	if (EnumHasAnyFlags(castFlags, CASTCLASS_FObjectProperty))
	{
		CollectHeapReference(collector, *static_cast<UObject**>(value));
	}
	else if (EnumHasAnyFlags(castFlags, CASTCLASS_FStructProperty))
	{
		CollectScriptStructReferences(collector, *static_cast<const FStructProperty&>(property).Struct.Get(), value);
	}
	else if (EnumHasAnyFlags(castFlags, CASTCLASS_FInterfaceProperty))
    {	
    	CollectInterfaceReference(collector, *static_cast<FScriptInterface*>(value));
    }
	else if (EnumHasAnyFlags(castFlags, CASTCLASS_FFieldPathProperty))
	{	
		CollectFieldPathReference(collector, *static_cast<FFieldPath*>(value));
	}
	else
	{
		checkNoEntry();
	}
}

void ZSharp::FZUnmanagedHeapGCHelper::CollectScriptStructReferences(FReferenceCollector& collector, const UScriptStruct& scriptStruct, void* instance)
{
	collector.AddPropertyReferencesWithStructARO(&scriptStruct, instance);
}

void ZSharp::FZUnmanagedHeapGCHelper::CollectArrayReferences(FReferenceCollector& collector, const FProperty& elementProperty, void* instance)
{
	// Migrate from CollectArrayReference() in UObjectGlobals.cpp.
	FScriptArrayHelper helper = FScriptArrayHelper::CreateHelperFormInnerProperty(&elementProperty, instance);
	if (ZUnmanagedHeapGCHelper_Private::MayContainStrongReference(elementProperty))
	{
		for (int32 i = 0; i < helper.Num(); ++i)
		{
			CollectPropertyReference(collector, elementProperty, helper.GetRawPtr(i));
		}
	}
}

void ZSharp::FZUnmanagedHeapGCHelper::CollectSetReferences(FReferenceCollector& collector, const FProperty& elementProperty, void* instance)
{
	// Migrate from CollectSetReference() in UObjectGlobals.cpp.
	FScriptSetHelper helper = FScriptSetHelper::CreateHelperFormElementProperty(const_cast<FProperty*>(&elementProperty), instance);
	if (ZUnmanagedHeapGCHelper_Private::MayContainStrongReference(elementProperty))
	{
		for (FScriptSetHelper::FIterator it(helper); it; ++it)
		{
			CollectPropertyReference(collector, elementProperty, helper.GetElementPtr(it));
		}
	}
}

void ZSharp::FZUnmanagedHeapGCHelper::CollectMapReferences(FReferenceCollector& collector, const FProperty& keyProperty, const FProperty& valueProperty, void* instance)
{
	// Migrate from CollectMapReference() in UObjectGlobals.cpp.
	FScriptMapHelper helper = FScriptMapHelper::CreateHelperFormInnerProperties(const_cast<FProperty*>(&keyProperty), const_cast<FProperty*>(&valueProperty), instance);
	if (ZUnmanagedHeapGCHelper_Private::MayContainStrongReference(keyProperty))
	{
		for (FScriptMapHelper::FIterator it(helper); it; ++it)
		{
			CollectPropertyReference(collector, keyProperty, helper.GetPairPtr(it));
		}
	}

	if (ZUnmanagedHeapGCHelper_Private::MayContainStrongReference(valueProperty))
	{
		for (FScriptMapHelper::FIterator it(helper); it; ++it)
		{
			CollectPropertyReference(collector, valueProperty, helper.GetPairPtr(it));
		}
	}
}

void ZSharp::FZUnmanagedHeapGCHelper::CollectOptionalReference(FReferenceCollector& collector, const FOptionalProperty& property, void* instance)
{
	// Migrate from CollectOptionalReference() in UObjectGlobals.cpp.
	const FProperty& valueProperty = *property.GetValueProperty();
	if (ZUnmanagedHeapGCHelper_Private::MayContainStrongReference(valueProperty))
	{
		if (void* value = property.GetValuePointerForReplaceIfSet(instance))
		{
			CollectPropertyReference(collector, valueProperty, value);		
		}
	}
}


