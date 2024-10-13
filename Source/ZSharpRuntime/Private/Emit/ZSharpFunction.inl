// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Reflection/Function/ZFunctionVisitor.h"
#include "Reflection/Function/ZFunctionVisitorRegistry.h"

namespace ZSharp::ZSharpFunction_Private
{
	static DEFINE_FUNCTION(execZCall)
	{
		FZFunctionVisitorRegistry::Get().Get(Stack.CurrentNativeFunction)->InvokeZCall(Context, Stack, RESULT_PARAM);
	}
}


