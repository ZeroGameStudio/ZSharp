// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Conjugate/ZConjugateHandle.h"

namespace ZSharp
{
	struct FZUnrealClass_Interop
	{
		static FZConjugateHandle GetDefaultObject(FZConjugateHandle self, uint8 createIfNeeded);
		static uint8 IsInterface(FZConjugateHandle self);
		static uint8 ImplementsInterface(FZConjugateHandle self, FZConjugateHandle interface);
	};
}


