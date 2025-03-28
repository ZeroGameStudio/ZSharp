﻿// Copyright Zero Games. All Rights Reserved.

#include "ZUnrealScriptStruct_Interop.h"

#include "CLR/IZSharpClr.h"
#include "Conjugate/ZStrangeConjugateRegistries.h"

void ZSharp::FZUnrealScriptStruct_Interop::Copy(FZConjugateHandle self, FZConjugateHandle other)
{
	FZConjugateRegistry_UScriptStruct& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>();
	FZSelfDescriptiveScriptStruct* sdself = registry.ConjugateUnsafe(self);
	FZSelfDescriptiveScriptStruct* sdother = registry.ConjugateUnsafe(other);
	if (ensure(sdother->GetDescriptor()->IsChildOf(sdself->GetDescriptor())))
	{
		sdself->GetDescriptor()->CopyScriptStruct(sdself->GetUnderlyingInstance(), sdother->GetUnderlyingInstance());
	}
}

uint8 ZSharp::FZUnrealScriptStruct_Interop::Identical(FZConjugateHandle self, FZConjugateHandle other)
{
	FZConjugateRegistry_UScriptStruct& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>();
	FZSelfDescriptiveScriptStruct* sdself = registry.ConjugateUnsafe(self);
	FZSelfDescriptiveScriptStruct* sdother = registry.ConjugateUnsafe(other);
	// Identical must have symmetry (A == B => B == A) so use == for descriptor.
	return sdself->GetDescriptor() == sdother->GetDescriptor() && sdself->GetDescriptor()->CompareScriptStruct(sdself->GetUnderlyingInstance(), sdother->GetUnderlyingInstance(), 0);
}


