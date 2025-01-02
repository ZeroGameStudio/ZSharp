// Copyright Zero Games. All Rights Reserved.

#include "ZWeakObjectPtr_Interop.h"

uint8 ZSharp::FZWeakObjectPtr_Interop::IsStale(FZConjugateHandle self, uint8 includingGarbage)
{
	const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_WeakObjectPtr>();
	const FZSelfDescriptiveWeakObjectPtr* sdself = registry.ConjugateUnsafe(self);
	return sdself->GetUnderlyingInstance()->IsStale(!!includingGarbage);
}


