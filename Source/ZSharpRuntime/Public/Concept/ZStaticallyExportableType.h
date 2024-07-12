// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Trait/ZIsStaticallyExportableType.h"

namespace ZSharp
{
	template <typename T>
	concept CZStaticallyExportableType = TZIsStaticallyExportableType_V<T>;
}


