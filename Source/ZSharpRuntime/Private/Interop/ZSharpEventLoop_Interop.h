// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSharpEventLoopEventType.h"

namespace ZSharp
{
	struct FZSharpEventLoop_Interop
	{
		inline static void(*GNotifyEvent)(EZSharpEventLoopEventType, float, float, double, double) = nullptr;
	};
}


