// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZIsUClass.h"
#include "ZIsUScriptStruct.h"

namespace ZSharp
{
	template <typename T>
	constexpr bool TZIsDynamicallyExportableClass_V = TZIsUClass_V<T> || TZIsUScriptStruct_V<T>;
}


