// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZObjectWrapperInteropBase.h"
#include "Conjugate/ZConjugateHandle.h"
#include "Conjugate/ZObjectWrapperConjugateRegisties.h"

namespace ZSharp
{
	struct FZSoftClassPtr_Interop : TZObjectWrapperInteropBase<FZConjugateRegistry_SoftClassPtr>
	{
		static uint8 IsPending(FZConjugateHandle self);
		static FZConjugateHandle Load(FZConjugateHandle self);
	};
}


