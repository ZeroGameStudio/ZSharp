// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Conjugate/ZConjugateHandle.h"

namespace ZSharp
{
	struct FZWeakObjectPtr_Interop
	{
		static void Copy(FZConjugateHandle self, FZConjugateHandle other);
		static uint8 Identical(FZConjugateHandle self, FZConjugateHandle other);
		static int32 Hash(FZConjugateHandle self);
		static FZConjugateHandle Get(FZConjugateHandle self, uint8 eventIfGarbage);
		static void Set(FZConjugateHandle self, FZConjugateHandle obj);
		static uint8 IsValid(FZConjugateHandle self, uint8 eventIfGarbage);
		static uint8 IsNull(FZConjugateHandle self);
		static uint8 IsStale(FZConjugateHandle self, uint8 includingGarbage);
	};
}


