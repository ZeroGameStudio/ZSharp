// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Trait/ZIsStaticExportableType.h"

namespace ZSharp
{
	template <typename T>
	concept CZStaticExportableType = TZIsStaticExportableType_V<T>;
}


