// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/ZGCHandle.h"

namespace ZSharp
{
	struct FZCallBuffer;
	
	struct FZMethodInfo_Interop
	{
		inline static uint8(*GGetName)(FZGCHandle, FString&) = nullptr;
		inline static int32(*GInvoke)(FZGCHandle, FZCallBuffer*) = nullptr;
	};
}


