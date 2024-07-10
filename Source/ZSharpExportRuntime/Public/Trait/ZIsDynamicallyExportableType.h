// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZIsDynamicallyExportedClass.h"
#include "ZIsDynamicallyExportableEnum.h"

namespace ZSharp
{
	template <typename T>
	constexpr bool TZIsDynamicallyExportableType_V = TZIsDynamicallyExportableClass_V<T> || TZIsDynamicallyExportableEnum_V<T>;
}


