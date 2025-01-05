// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Conjugate/ZConjugateHandle.h"

namespace ZSharp
{
	struct FZUnrealObject_Interop
	{
		static FZConjugateHandle GetClass(FZConjugateHandle self);
		static void GetName(FZConjugateHandle self, FString& result);
		static void GetPathName(FZConjugateHandle self, FString& result);
		static FZConjugateHandle GetOuter(FZConjugateHandle self);
		static FZConjugateHandle GetWorld(FZConjugateHandle self);
		static uint8 IsA(FZConjugateHandle self, FZConjugateHandle cls);
		static uint8 IsIn(FZConjugateHandle self, FZConjugateHandle outer);
		static uint8 Rename(FZConjugateHandle self, const TCHAR* newName, FZConjugateHandle newOuter);
	};
}


