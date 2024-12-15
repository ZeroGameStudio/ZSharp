// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Conjugate/ZConjugateHandle.h"

namespace ZSharp
{
	struct FZInputActionValue_Interop
	{
		static void Deconstruct(FZConjugateHandle self, double& x, double& y, double& z);
	};
}


