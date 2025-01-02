// Copyright Zero Games. All Rights Reserved.

#include "ZLazyObjectPtr_Interop.h"

uint8 ZSharp::FZLazyObjectPtr_Interop::IsPending(FZConjugateHandle self)
{
	const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_LazyObjectPtr>();
	const FZSelfDescriptiveLazyObjectPtr* sdself = registry.ConjugateUnsafe(self);
	return sdself->GetUnderlyingInstance()->IsPending();
}


