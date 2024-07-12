// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Trait/ZIsStaticallyExportableEnum.h"

namespace ZSharp
{
	template <typename T>
	concept CZStaticallyExportableEnum = TZIsStaticallyExportableEnum_V<T>;
}


