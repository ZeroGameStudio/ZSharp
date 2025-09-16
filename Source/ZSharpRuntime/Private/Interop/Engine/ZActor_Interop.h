// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Conjugate/ZConjugateHandle.h"

namespace ZSharp
{
	struct FZActor_Interop
	{
		static void FinishSpawning(FZConjugateHandle self, FZConjugateHandle transform);
		static FZConjugateHandle AddComponent(FZConjugateHandle self, FZConjugateHandle componentClass, uint8 defer);
		static void FinishAddComponent(FZConjugateHandle self, FZConjugateHandle component);
		static ENetMode GetNetMode(FZConjugateHandle self);
	};
}


