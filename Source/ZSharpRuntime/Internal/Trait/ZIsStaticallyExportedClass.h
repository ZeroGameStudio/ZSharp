// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZIsUClass.h"
#include "ZIsUScriptStruct.h"

namespace ZSharp
{
	template <typename T>
	constexpr bool TZIsStaticallyExportableClass_V = TIsClass<T>::Value && !TZIsUClass_V<T> && !TZIsUScriptStruct_V<T>;
}


