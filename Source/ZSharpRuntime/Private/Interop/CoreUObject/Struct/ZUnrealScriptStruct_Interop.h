// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Conjugate/ZConjugateHandle.h"

namespace ZSharp
{
	struct FZUnrealScriptStruct_Interop
	{
		static void Copy(FZConjugateHandle self, FZConjugateHandle other);
		static uint8 Identical(FZConjugateHandle self, FZConjugateHandle other);
	};
}


