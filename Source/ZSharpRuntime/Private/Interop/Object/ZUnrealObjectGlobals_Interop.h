// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Conjugate/ZConjugateHandle.h"

namespace ZSharp
{
	struct FZUnrealObjectGlobals_Interop
	{
		static FZConjugateHandle NewObject(FZConjugateHandle cls, FZConjugateHandle outer, const TCHAR* name);
		static FZConjugateHandle LoadObject(FZConjugateHandle cls, FZConjugateHandle outer, const TCHAR* name);
		static FZConjugateHandle DuplicateObject(FZConjugateHandle source, FZConjugateHandle outer, const TCHAR* name, FZConjugateHandle cls);
		static FZConjugateHandle FindObject(FZConjugateHandle cls, FZConjugateHandle outer, const TCHAR* name, uint8 exactClass);
		static FZConjugateHandle LowLevelFindObject(const TCHAR* path);
		static FZConjugateHandle GetTransientPackage();
	};
}


