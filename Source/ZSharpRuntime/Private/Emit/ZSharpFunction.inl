// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSharpFunctionFailureDelegate.h"
#include "Reflection/Function/ZFunctionVisitor.h"
#include "Reflection/Function/ZFunctionVisitorRegistry.h"

namespace ZSharp::ZSharpFunction_Private
{
	static DEFINE_FUNCTION(execZCall)
	{
		EZCallErrorCode error = FZFunctionVisitorRegistry::Get().Get(Stack.CurrentNativeFunction)->InvokeZCall(Context, Stack, RESULT_PARAM);
		if (error != EZCallErrorCode::Succeed)
		{
			FZSharpFunctionFailureDelegate::Get().Execute(error, Context, Stack, RESULT_PARAM);
		}
	}
}


