// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ALC/ZRuntimeTypeUri.h"

namespace ZSharp
{
	struct ZSHARPRUNTIME_API FZReflectionHelper
	{
		static FString GetFieldConjugateKey(const UField* field);
		static FString GetFieldClassConjugateKey(const FFieldClass* cls);
		static FZRuntimeTypeUri GetContainerElementRuntimeTypeUriFromProperty(const FProperty* property);
	};
}


