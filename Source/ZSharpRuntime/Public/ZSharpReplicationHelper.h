// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "UObject/CoreNet.h"

namespace ZSharp
{
	struct ZSHARPRUNTIME_API FZSharpReplicationHelper
	{
		static void GetZSharpLifetimeReplicatedProps(const UObject* object, TArray<FLifetimeProperty>& outLifetimeProps);
	};
}


