// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/ZGCHandle.h"

namespace ZSharp
{
	struct ZCLR_Interop
	{
		inline static void(*GCollectGarbage)(int32, uint8, uint8, uint8) = nullptr;
		inline static FZGCHandle(*GCreateMasterALC)() = nullptr;
		inline static FZGCHandle(*GCreateSlimALC)(const TCHAR*) = nullptr;
	};
}
