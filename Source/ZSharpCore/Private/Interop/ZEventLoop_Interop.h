// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Misc/Async/ZEventLoopTickingGroup.h"

namespace ZSharp
{
	struct FZEventLoop_Interop
	{
		inline static void(*GNotifyEvent)(EZEventLoopTickingGroup, float, float, double, double) = nullptr;
	};
}


