// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/ZGCHandle.h"

namespace ZSharp
{
	struct FZAssembly_Interop
	{
		inline static uint8(*GGetName)(FZGCHandle, FString&) = nullptr;
	};
}


