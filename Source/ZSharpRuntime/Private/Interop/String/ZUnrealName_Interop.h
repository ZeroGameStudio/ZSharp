// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Conjugate/ZConjugateHandle.h"

namespace ZSharp
{
	struct FZUnrealName_Interop
	{
		static void GetData(FZConjugateHandle self, FString& result);
		static void SetData(FZConjugateHandle self, const TCHAR* data);
	};
}


