// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Trait/ZIsStaticallyExportedClass.h"

namespace ZSharp
{
	template <typename T>
	concept CZStaticallyExportableClass = TZIsStaticallyExportableClass_V<T>;
}


