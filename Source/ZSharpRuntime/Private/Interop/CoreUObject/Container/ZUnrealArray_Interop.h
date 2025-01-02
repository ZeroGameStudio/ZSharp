// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Conjugate/ZConjugateHandle.h"
#include "ZCall/ZCallBuffer.h"

namespace ZSharp
{
	struct FZUnrealArray_Interop
	{
		static void Insert(FZConjugateHandle self, int32 index, FZCallBufferSlot item);
		static uint8 Remove(FZConjugateHandle self, FZCallBufferSlot item);
		static void RemoveAt(FZConjugateHandle self, int32 index);
		static void Clear(FZConjugateHandle self);
		static int32 IndexOf(FZConjugateHandle self, FZCallBufferSlot item);
		static void Get(FZConjugateHandle self, int32 index, FZCallBufferSlot& item);
		static void Set(FZConjugateHandle self, int32 index, FZCallBufferSlot item);
		static int32 Count(FZConjugateHandle self);
	};
}


