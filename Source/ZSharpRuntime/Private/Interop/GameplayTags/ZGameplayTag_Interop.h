// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Conjugate/ZConjugateHandle.h"

namespace ZSharp
{
	struct FZGameplayTag_Interop
	{
		static FZConjugateHandle Request(const TCHAR* tagName);
	};
}


