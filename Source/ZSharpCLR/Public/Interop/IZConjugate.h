// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZGCHandle.h"

namespace ZSharp
{
	class ZSHARPCLR_API IZConjugate : public IZGCHandle
	{
	public:
		virtual void Free() = 0;
	};
}


