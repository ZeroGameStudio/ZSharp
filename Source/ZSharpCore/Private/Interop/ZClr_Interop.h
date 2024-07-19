// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/ZGCHandle.h"

namespace ZSharp
{
	struct FZClr_Interop
	{
		inline static void(*GCollectGarbage)(int32, uint8, uint8, uint8) = nullptr;
		inline static FZGCHandle(*GCreateMasterAlc)() = nullptr;
		inline static FZGCHandle(*GCreateSlimAlc)(const TCHAR*) = nullptr;
	};
}
