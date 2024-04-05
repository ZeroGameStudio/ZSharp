// Copyright Zero Games. All Rights Reserved.

#pragma once

#if ZSHARP_WITH_CORECLR

#include "GenericCLR.h"

namespace ZSharp
{
	class ZSHARPCLR_API FCoreCLR : public FGenericCLR
	{
		
	public:
		static FCoreCLR& Get();
		
	};
}

#endif


