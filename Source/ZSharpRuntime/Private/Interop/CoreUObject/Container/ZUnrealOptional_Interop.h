// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Conjugate/ZConjugateHandle.h"
#include "ZCall/ZCallBuffer.h"

namespace ZSharp
{
	struct FZUnrealOptional_Interop
	{
		static uint8 Get(FZConjugateHandle self, FZCallBufferSlot& value);
		static void Set(FZConjugateHandle self, FZCallBufferSlot value);
		static void Reset(FZConjugateHandle self);
		static uint8 IsSet(FZConjugateHandle self);
	};
}


