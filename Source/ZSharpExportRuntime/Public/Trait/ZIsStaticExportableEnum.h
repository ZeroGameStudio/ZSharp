// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZIsUEnum.h"

namespace ZSharp
{
	template <typename T>
	constexpr bool TZIsStaticExportableEnum_V = TIsEnum<T>::Value && !TZIsUEnum_V<T>;
}


