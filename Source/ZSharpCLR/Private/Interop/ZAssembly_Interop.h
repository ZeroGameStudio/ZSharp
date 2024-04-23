// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/ZGCHandle.h"

namespace ZSharp
{
	struct FZAssembly_Interop
	{
		inline static int32(*GGetName)(FZGCHandle, FString&) = nullptr;
		inline static FZGCHandle(*GGetType)(FZGCHandle, const TCHAR*) = nullptr;
	};
}


