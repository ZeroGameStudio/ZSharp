// Copyright Zero Games. All Rights Reserved.

#include "ZUnrealScriptStructBase_Interop.h"

#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"
#include "Conjugate/ZStrangeConjugateRegistries.h"
#include "Interop/ZInteropExceptionHelper.h"

void ZSharp::FZUnrealScriptStructBase_Interop::Copy(FZConjugateHandle self, FZConjugateHandle other)
{
	GUARD
	(
		FZConjugateRegistry_UScriptStruct& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>();
		FZSelfDescriptiveScriptStruct* sdself = registry.ConjugateUnsafe(self);
		FZSelfDescriptiveScriptStruct* sdother = registry.ConjugateUnsafe(other);
		if (ensure(sdother->GetDescriptor()->IsChildOf(sdself->GetDescriptor())))
		{
			sdself->GetDescriptor()->CopyScriptStruct(sdself->GetUnderlyingInstance(), sdother->GetUnderlyingInstance());
		}
	);
}

uint8 ZSharp::FZUnrealScriptStructBase_Interop::Identical(FZConjugateHandle self, FZConjugateHandle other)
{
	TRY
	{
		FZConjugateRegistry_UScriptStruct& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>();
		FZSelfDescriptiveScriptStruct* sdself = registry.ConjugateUnsafe(self);
		FZSelfDescriptiveScriptStruct* sdother = registry.ConjugateUnsafe(other);
		// Identical must have symmetry (A == B => B == A) so use == for descriptor.
		return sdself->GetDescriptor() == sdother->GetDescriptor() && sdself->GetDescriptor()->CompareScriptStruct(sdself->GetUnderlyingInstance(), sdother->GetUnderlyingInstance(), 0);
	}
	CATCHR(false)
}

ZSharp::FZConjugateHandle ZSharp::FZUnrealScriptStructBase_Interop::GetScriptStruct(FZConjugateHandle self)
{
	TRY
	{
		FZConjugateRegistry_UScriptStruct& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>();
		FZSelfDescriptiveScriptStruct* sdself = registry.ConjugateUnsafe(self);
		return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().Conjugate(sdself->GetDescriptor());
	}
	CATCHR({})
}

uint8 ZSharp::FZUnrealScriptStructBase_Interop::IsA(FZConjugateHandle self, FZConjugateHandle scriptStruct)
{
	TRY
	{
		FZConjugateRegistry_UScriptStruct& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>();
		FZSelfDescriptiveScriptStruct* sdself = registry.ConjugateUnsafe(self);
		auto pScriptStruct = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafeChecked<const UScriptStruct>(scriptStruct);
		return sdself->GetDescriptor()->IsChildOf(pScriptStruct);
	}
	CATCHR(false)
}


