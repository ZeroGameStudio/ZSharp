// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Conjugate/ZConjugateHandle.h"

class FNativeGameplayTag;

namespace ZSharp
{
	struct FZGameplayTag_Interop
	{
		static FNativeGameplayTag* TryRegisterNativeTag(const TCHAR* tagName, const TCHAR* comment, const TCHAR* pluginName, const TCHAR* moduleName);
		static uint8 TryUnregisterNativeTag(FNativeGameplayTag* tag);
		
		static FZConjugateHandle Request(const TCHAR* tagName);
	};
}


