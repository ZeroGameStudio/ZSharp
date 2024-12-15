// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Conjugate/ZConjugateHandle.h"
#include "Interop/ZGCHandle.h"

namespace ZSharp
{
	struct FZEnhancedInputComponent_Interop
	{
		static FZConjugateHandle BindAction(FZConjugateHandle self, FZConjugateHandle inputAction, int64 triggerEvent, FZGCHandle delegate);
		static uint8 RemoveBinding(FZConjugateHandle self, FZConjugateHandle handle);
	};
}


