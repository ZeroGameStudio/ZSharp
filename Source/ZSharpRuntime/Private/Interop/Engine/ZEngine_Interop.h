// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Conjugate/ZConjugateHandle.h"

namespace ZSharp
{
	struct FZEngine_Interop
	{
		static uint8 IsInitialized();
		static FZConjugateHandle GetEngine();

		inline static void(*GPostEngineInit)() = nullptr;
	};
}


