// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Conjugate/ZConjugateHandle.h"
#include "ZCall/ZCallBuffer.h"

namespace ZSharp
{
	struct FZUnrealMap_Interop
	{
		struct FZIterator
		{
			explicit FZIterator(const FScriptMapHelper& target);
			FScriptMapHelper Target;
			TUniquePtr<FScriptMapHelper::FIterator> Iterator;
		};

	public:
		static uint8 TryAdd(FZConjugateHandle self, FZCallBufferSlot key, FZCallBufferSlot value);
		static uint8 Remove(FZConjugateHandle self, FZCallBufferSlot key);
		static void Clear(FZConjugateHandle self);
		static uint8 Contains(FZConjugateHandle self, FZCallBufferSlot key);
		static uint8 Get(FZConjugateHandle self, FZCallBufferSlot key, FZCallBufferSlot& value);
		static void Set(FZConjugateHandle self, FZCallBufferSlot key, FZCallBufferSlot value);
		static int32 Count(FZConjugateHandle self);

		static FZIterator* CreateEnumerator(FZConjugateHandle target);
		static void ReleaseEnumerator(FZIterator* self);
		static uint8 EnumeratorMoveNext(FZIterator* self);
		static void EnumeratorCurrent(FZIterator* self, FZConjugateHandle target, FZCallBufferSlot& key, FZCallBufferSlot& value);
	};
}


