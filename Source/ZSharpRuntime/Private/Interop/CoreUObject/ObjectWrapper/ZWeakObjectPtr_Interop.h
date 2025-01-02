// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZObjectWrapperInteropBase.h"
#include "Conjugate/ZObjectWrapperConjugateRegisties.h"

namespace ZSharp
{
	struct FZWeakObjectPtr_Interop : TZObjectWrapperInteropBase<FZConjugateRegistry_WeakObjectPtr>
	{
		static uint8 IsStale(FZConjugateHandle self, uint8 includingGarbage);
	};
}


