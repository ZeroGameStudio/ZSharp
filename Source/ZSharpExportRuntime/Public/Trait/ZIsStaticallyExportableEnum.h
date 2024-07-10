// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZIsUEnum.h"

namespace ZSharp
{
	template <typename T>
	constexpr bool TZIsStaticallyExportableEnum_V = TIsEnum<T>::Value && !TZIsUEnum_V<T>;
}


