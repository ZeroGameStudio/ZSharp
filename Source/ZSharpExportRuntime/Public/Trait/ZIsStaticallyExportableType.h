// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZIsStaticallyExportableEnum.h"
#include "ZIsStaticallyExportedClass.h"

namespace ZSharp
{
	template <typename T>
	constexpr bool TZIsStaticallyExportableType_V = TZIsStaticallyExportableClass_V<T> || TZIsStaticallyExportableEnum_V<T>;
}


