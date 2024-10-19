// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSharpErrorCodes.h"

namespace ZSharp
{
	struct ZSHARPRUNTIME_API FZHookZSharpFunctionFailure
	{
		explicit FZHookZSharpFunctionFailure(TFunction<void(EZCallErrorCode, UObject*, FFrame&, void* const)> hook);
	};
}


