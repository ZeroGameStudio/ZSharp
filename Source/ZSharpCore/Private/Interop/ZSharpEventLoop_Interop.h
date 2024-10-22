// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Misc/ZSharpEventLoopTickingGroup.h"

namespace ZSharp
{
	struct FZSharpEventLoop_Interop
	{
		inline static void(*GNotifyEvent)(EZSharpEventLoopTickingGroup, float, float, double, double) = nullptr;
	};
}


