// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Trait/ZIsStaticExportableEnum.h"

namespace ZSharp
{
	template <typename T>
	concept CZStaticExportableEnum = TZIsStaticExportableEnum_V<T>;
}


