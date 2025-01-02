// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Conjugate/ZConjugateHandle.h"

namespace ZSharp
{
	struct FZStrongObjectPtr_Interop
	{
		using FStrongObjectPtr = TStrongObjectPtr<UObject>;

		static FStrongObjectPtr* Alloc(FZConjugateHandle target);
		static void Free(FStrongObjectPtr* self);

		static uint8 Identical(FStrongObjectPtr* self, FStrongObjectPtr* other);
		static int32 Hash(FStrongObjectPtr* self);

		static FZConjugateHandle Get(FStrongObjectPtr* self, uint8 evenIfGarbage);
		static void Set(FStrongObjectPtr* self, FZConjugateHandle target);
		static uint8 IsValid(FStrongObjectPtr* self, uint8 evenIfGarbage);
		static uint8 IsNull(FStrongObjectPtr* self);
	};
}


