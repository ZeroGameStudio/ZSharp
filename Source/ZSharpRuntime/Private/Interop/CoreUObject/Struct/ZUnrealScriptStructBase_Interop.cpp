// Copyright Zero Games. All Rights Reserved.

#include "ZUnrealScriptStructBase_Interop.h"

#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"
#include "Conjugate/ZStrangeConjugateRegistries.h"

void ZSharp::FZUnrealScriptStructBase_Interop::Copy(FZConjugateHandle self, FZConjugateHandle other)
{
	FZConjugateRegistry_UScriptStruct& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>();
	FZSelfDescriptiveScriptStruct* sdself = registry.ConjugateUnsafe(self);
	FZSelfDescriptiveScriptStruct* sdother = registry.ConjugateUnsafe(other);
	if (ensure(sdother->GetDescriptor()->IsChildOf(sdself->GetDescriptor())))
	{
		sdself->GetDescriptor()->CopyScriptStruct(sdself->GetUnderlyingInstance(), sdother->GetUnderlyingInstance());
	}
}

uint8 ZSharp::FZUnrealScriptStructBase_Interop::Identical(FZConjugateHandle self, FZConjugateHandle other)
{
	FZConjugateRegistry_UScriptStruct& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>();
	FZSelfDescriptiveScriptStruct* sdself = registry.ConjugateUnsafe(self);
	FZSelfDescriptiveScriptStruct* sdother = registry.ConjugateUnsafe(other);
	// Identical must have symmetry (A == B => B == A) so use == for descriptor.
	return sdself->GetDescriptor() == sdother->GetDescriptor() && sdself->GetDescriptor()->CompareScriptStruct(sdself->GetUnderlyingInstance(), sdother->GetUnderlyingInstance(), 0);
}

ZSharp::FZConjugateHandle ZSharp::FZUnrealScriptStructBase_Interop::GetScriptStruct(FZConjugateHandle self)
{
	FZConjugateRegistry_UScriptStruct& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>();
	FZSelfDescriptiveScriptStruct* sdself = registry.ConjugateUnsafe(self);
	return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().Conjugate(sdself->GetDescriptor());
}

uint8 ZSharp::FZUnrealScriptStructBase_Interop::IsA(FZConjugateHandle self, FZConjugateHandle scriptStruct)
{
	FZConjugateRegistry_UScriptStruct& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>();
	FZSelfDescriptiveScriptStruct* sdself = registry.ConjugateUnsafe(self);
	auto pScriptStruct = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafeChecked<const UScriptStruct>(scriptStruct);
	return sdself->GetDescriptor()->IsChildOf(pScriptStruct);
}


