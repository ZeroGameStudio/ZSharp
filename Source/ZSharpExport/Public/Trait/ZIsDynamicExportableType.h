// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZIsDynamicExportedClass.h"
#include "ZIsDynamicExportableEnum.h"

namespace ZSharp
{
	template <typename T>
	constexpr bool TZIsDynamicExportableType_V = TZIsDynamicExportableClass_V<T> || TZIsDynamicExportableEnum_V<T>;
}


