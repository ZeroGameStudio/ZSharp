// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Conjugate/ZConjugateHandle.h"

namespace ZSharp
{
	struct FZUnrealString_Interop
	{
		static const TCHAR* GetData(FZConjugateHandle self);
		static void SetData(FZConjugateHandle self, const TCHAR* data);
	};
}


