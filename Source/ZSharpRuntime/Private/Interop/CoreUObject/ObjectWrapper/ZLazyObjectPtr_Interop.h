// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZObjectWrapperInteropBase.h"
#include "Conjugate/ZConjugateHandle.h"
#include "Conjugate/ZObjectWrapperConjugateRegisties.h"

namespace ZSharp
{
	struct FZLazyObjectPtr_Interop : TZObjectWrapperInteropBase<FZConjugateRegistry_LazyObjectPtr>
	{
		static uint8 IsPending(FZConjugateHandle self);
	};
}


