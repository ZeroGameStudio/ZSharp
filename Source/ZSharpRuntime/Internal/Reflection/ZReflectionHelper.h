// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ALC/ZRuntimeTypeUri.h"

namespace ZSharp
{
	struct ZSHARPRUNTIME_API FZReflectionHelper
	{
		static FString GetFieldConjugateKey(const UField* field);
		static FString GetFieldClassConjugateKey(const FFieldClass* cls);
		static FZRuntimeTypeUri GetContainerElementRuntimeTypeUriFromProperty(const FProperty* elementProperty);
		static void ChangeEnumValueType(const FEnumProperty* srcProperty, const void* src, const FEnumProperty* destProperty, void* dest);
		static bool CanPropertyBeReference(const FProperty* property);
	};
}


