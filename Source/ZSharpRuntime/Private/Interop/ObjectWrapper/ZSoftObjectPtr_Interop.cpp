// Copyright Zero Games. All Rights Reserved.

#include "ZSoftObjectPtr_Interop.h"

uint8 ZSharp::FZSoftObjectPtr_Interop::IsPending(FZConjugateHandle self)
{
	const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_SoftObjectPtr>();
	const FZSelfDescriptiveSoftObjectPtr* sdself = registry.ConjugateUnsafe(self);
	return sdself->GetUnderlyingInstance()->IsPending();
}

ZSharp::FZConjugateHandle ZSharp::FZSoftObjectPtr_Interop::Load(FZConjugateHandle self)
{
	const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_SoftObjectPtr>();
	const FZSelfDescriptiveSoftObjectPtr* sdself = registry.ConjugateUnsafe(self);
	return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().Conjugate(sdself->GetUnderlyingInstance()->LoadSynchronous());
}


