// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/ZGCHandle.h"

namespace ZSharp
{
	struct FZGCHandle_Interop
	{
		inline static void(*GFree)(FZGCHandle) = nullptr;
	};
}


