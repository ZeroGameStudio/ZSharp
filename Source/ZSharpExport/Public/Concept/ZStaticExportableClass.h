// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Trait/ZIsStaticExportedClass.h"

namespace ZSharp
{
	template <typename T>
	concept CZStaticExportableClass = TZIsStaticExportableClass_V<T>;
}


