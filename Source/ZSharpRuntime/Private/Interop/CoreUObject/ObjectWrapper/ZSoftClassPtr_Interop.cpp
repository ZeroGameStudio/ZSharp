﻿// Copyright Zero Games. All Rights Reserved.

#include "ZSoftClassPtr_Interop.h"

uint8 ZSharp::FZSoftClassPtr_Interop::IsPending(FZConjugateHandle self)
{
	const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_SoftClassPtr>();
	const FZSelfDescriptiveSoftClassPtr* sdself = registry.ConjugateUnsafe(self);
	return sdself->GetUnderlyingInstance()->IsPending();
}

ZSharp::FZConjugateHandle ZSharp::FZSoftClassPtr_Interop::Load(FZConjugateHandle self)
{
	const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_SoftClassPtr>();
	const FZSelfDescriptiveSoftClassPtr* sdself = registry.ConjugateUnsafe(self);
	return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().Conjugate(sdself->GetUnderlyingInstance()->LoadSynchronous());
}


