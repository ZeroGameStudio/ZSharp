// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/ZGCHandle.h"

namespace ZSharp
{
	struct FZType_Interop
	{
		inline static int32(*GGetName)(FZGCHandle, FString&) = nullptr;
		inline static FZConjugateHandle(*GBuildConjugate)(FZGCHandle, void*) = nullptr;
		inline static FZGCHandle(*GGetMethodInfo)(FZGCHandle, const TCHAR*) = nullptr;
		inline static FZGCHandle(*GGetPropertyInfo)(FZGCHandle, const TCHAR*) = nullptr;
	};
}


