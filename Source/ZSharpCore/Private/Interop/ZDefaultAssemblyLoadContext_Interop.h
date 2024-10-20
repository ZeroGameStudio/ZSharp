// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSharpErrorCodes.h"

namespace ZSharp
{
	struct FZDefaultAssemblyLoadContext_Interop
	{
		inline static EZLoadAssemblyErrorCode(*GLoadAssembly)(const TCHAR*, void*) = nullptr;
		inline static EZInvokeMethodErrorCode(*GInvokeMethod)(const TCHAR*, const TCHAR*, const TCHAR*, void*) = nullptr;
	};
}


