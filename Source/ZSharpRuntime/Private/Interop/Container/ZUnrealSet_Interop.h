// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Conjugate/ZConjugateHandle.h"
#include "ZCall/ZCallBuffer.h"

namespace ZSharp
{
	struct FZUnrealSet_Interop
	{
	private:
		struct FZIterator
		{
			explicit FZIterator(const FScriptSetHelper& target);
			FScriptSetHelper Target;
			TUniquePtr<FScriptSetHelper::FIterator> Iterator;
		};

	public:
		static uint8 Add(FZConjugateHandle self, FZCallBufferSlot item);
		static uint8 Remove(FZConjugateHandle self, FZCallBufferSlot item);
		static void Clear(FZConjugateHandle self);
		static uint8 Contains(FZConjugateHandle self, FZCallBufferSlot item);
		static int32 Count(FZConjugateHandle self);

		static FZIterator* CreateEnumerator(FZConjugateHandle target);
		static void ReleaseEnumerator(FZIterator* self);
		static uint8 EnumeratorMoveNext(FZIterator* self);
		static void EnumeratorCurrent(FZIterator* self, FZConjugateHandle target, FZCallBufferSlot& item);
		static uint8 EnumeratorIsValid(FZIterator* self);
	};
}


