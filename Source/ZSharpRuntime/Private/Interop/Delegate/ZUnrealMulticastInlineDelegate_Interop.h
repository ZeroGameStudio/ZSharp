// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSharpErrorCodes.h"
#include "Conjugate/ZConjugateHandle.h"
#include "Interop/ZGCHandle.h"

namespace ZSharp
{
	struct FZCallBuffer;
	
	struct FZUnrealMulticastInlineDelegate_Interop
	{
		static EZCallErrorCode Broadcast(FZCallBuffer* buffer);
		static FZConjugateHandle AddManagedDelegate(FZConjugateHandle self, FZGCHandle delegate);
		static void AddUnrealFunction(FZConjugateHandle self, FZConjugateHandle object, const TCHAR* functionName);
		static void Remove(FZConjugateHandle self, FZConjugateHandle object, const TCHAR* functionName);
		static void RemoveAll(FZConjugateHandle self, FZConjugateHandle object);
		static void Clear(FZConjugateHandle self);
		static uint8 IsBound(FZConjugateHandle self);
		static uint8 IsBoundToObject(FZConjugateHandle self, FZConjugateHandle object);
		static uint8 Contains(FZConjugateHandle self, FZConjugateHandle object, const TCHAR* functionName);
	};
}


