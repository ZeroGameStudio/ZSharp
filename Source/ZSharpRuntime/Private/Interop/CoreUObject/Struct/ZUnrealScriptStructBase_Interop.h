// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Conjugate/ZConjugateHandle.h"

namespace ZSharp
{
	struct FZUnrealScriptStructBase_Interop
	{
		static void Copy(FZConjugateHandle self, FZConjugateHandle other);
		static uint8 Identical(FZConjugateHandle self, FZConjugateHandle other);

		static FZConjugateHandle GetScriptStruct(FZConjugateHandle self);
		static uint8 IsA(FZConjugateHandle self, FZConjugateHandle scriptStruct);
	};
}


