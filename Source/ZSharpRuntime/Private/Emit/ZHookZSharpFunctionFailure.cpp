// Copyright Zero Games. All Rights Reserved.


#include "Emit/ZHookZSharpFunctionFailure.h"

#include "ZSharpFunctionFailureDelegate.h"

ZSharp::FZHookZSharpFunctionFailure::FZHookZSharpFunctionFailure(TFunction<void(EZCallErrorCode, UObject*, FFrame&, void* const)> hook)
{
	FZSharpFunctionFailureDelegate::Get().Hook(hook);
}


