// Copyright Zero Games. All Rights Reserved.

#include "ZSharpFunctionFailureDelegate.h"

ZSharp::FZSharpFunctionFailureDelegate& ZSharp::FZSharpFunctionFailureDelegate::Get()
{
	static FZSharpFunctionFailureDelegate GInstance;

	return GInstance;
}

void ZSharp::FZSharpFunctionFailureDelegate::Execute(EZCallErrorCode error, UObject* object, FFrame& stack, RESULT_DECL)
{
	if (HookFunction)
	{
		HookFunction(error, object, stack, RESULT_PARAM);
	}
}

void ZSharp::FZSharpFunctionFailureDelegate::Hook(TFunction<void(EZCallErrorCode, UObject*, FFrame&, void* const)> hook)
{
	check(!HookFunction);
	HookFunction = hook;
}


