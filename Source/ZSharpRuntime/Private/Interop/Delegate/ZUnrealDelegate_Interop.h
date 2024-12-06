// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSharpErrorCodes.h"
#include "Conjugate/ZConjugateHandle.h"
#include "Interop/ZGCHandle.h"

namespace ZSharp
{
	struct FZCallBuffer;
	
	struct FZUnrealDelegate_Interop
	{
		static EZCallErrorCode Execute(FZCallBuffer* buffer);
		static FZConjugateHandle BindManagedDelegate(FZConjugateHandle self, FZGCHandle delegate);
		static void BindUnrealFunction(FZConjugateHandle self, FZConjugateHandle object, const TCHAR* functionName);
		static void Unbind(FZConjugateHandle self);
		static uint8 IsBound(FZConjugateHandle self);
		static uint8 IsBoundToObject(FZConjugateHandle self, FZConjugateHandle object);
		static FZConjugateHandle GetObject(FZConjugateHandle self);
		static void GetFunctionName(FZConjugateHandle self, FString& functionName);
	};
}


