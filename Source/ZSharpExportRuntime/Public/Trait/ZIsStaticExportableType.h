// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZIsStaticExportableEnum.h"
#include "ZIsStaticExportedClass.h"

namespace ZSharp
{
	template <typename T>
	constexpr bool TZIsStaticExportableType_V = TZIsStaticExportableClass_V<T> || TZIsStaticExportableEnum_V<T>;
}


