// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	ZSHARPRUNTIME_API struct FRegisterCustomThunk
	{
		FRegisterCustomThunk(FName name, FNativeFuncPtr thunk);
	};
}


