// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	/**
 	 * !!!!!!!!!!!!!!!!!!!!! LOW LEVEL HACK !!!!!!!!!!!!!!!!!!!!!
 	 * 
 	 * VERY, VERY, VERY IMPORTANT:
 	 * THIS RELIES HEAVILY ON IMPLEMENTATION DETAILS OF UNREAL ENGINE,
 	 * ESPECIALLY THE GARBAGE COLLECTOR.
 	 * THEREFORE, UPGRADING THE ENGINE VERSION OR MODIFYING THE CODE IS HIGHLY LIKELY TO BREAK SOMETHING UP.
 	 * NEVER TOUCH THIS CODE UNLESS YOU EXTREMELY, EXTREMELY, EXTREMELY KNOW WHAT YOU ARE DOING!!!
 	 *
 	 * !!!!!!!!!!!!!!!!!!!!! LOW LEVEL HACK !!!!!!!!!!!!!!!!!!!!!
 	 */
	struct FZUnmanagedHeapGCHelper
	{
		static bool CollectHeapReference(FReferenceCollector& collector, UObject*& reference);
		static void CollectInterfaceReference(FReferenceCollector& collector, FScriptInterface& instance);
		static void CollectFieldPathReference(FReferenceCollector& collector, FFieldPath& instance);
		static void CollectPropertyReference(FReferenceCollector& collector, const FProperty& property, void* value);
		static void CollectScriptStructReferences(FReferenceCollector& collector, const UScriptStruct& scriptStruct, void* instance);
		static void CollectArrayReferences(FReferenceCollector& collector, const FProperty& elementProperty, void* instance);
		static void CollectSetReferences(FReferenceCollector& collector, const FProperty& elementProperty, void* instance);
		static void CollectMapReferences(FReferenceCollector& collector, const FProperty& keyProperty, const FProperty& valueProperty, void* instance);
		static void CollectOptionalReference(FReferenceCollector& collector, const FOptionalProperty& property, void* instance);
	};
}


