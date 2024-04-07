// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/ZGCHandle.h"

namespace ZSharp
{
	struct ZCLR_Interop
	{
		inline static FZGCHandle(*GCreateMasterALC)() = nullptr;
	};
}
