// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/ZGCHandle.h"

namespace ZSharp
{
	struct FZSlimAssemblyLoadContext_Interop
	{
		inline static int32(*GUnload)(FZGCHandle) = nullptr;
		inline static int32(*GLoadAssembly)(FZGCHandle, const uint8*, int32, void*) = nullptr;
		inline static int32(*GCallMethod)(FZGCHandle, const TCHAR*, const TCHAR*, const TCHAR*, void*) = nullptr;
	};
}


