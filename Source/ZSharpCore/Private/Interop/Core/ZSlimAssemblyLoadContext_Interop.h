// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSharpErrorCodes.h"
#include "Interop/ZGCHandle.h"

namespace ZSharp
{
	struct FZSlimAssemblyLoadContext_Interop
	{
		inline static int32(*GUnload)(FZGCHandle) = nullptr;
		inline static EZLoadAssemblyErrorCode(*GLoadAssembly)(FZGCHandle, const TCHAR*, void*) = nullptr;
		inline static EZInvokeMethodErrorCode(*GInvokeMethod)(FZGCHandle, const TCHAR*, const TCHAR*, const TCHAR*, void*) = nullptr;
	};
}


