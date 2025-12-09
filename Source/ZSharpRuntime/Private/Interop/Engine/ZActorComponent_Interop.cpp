// Copyright Zero Games. All Rights Reserved.

#include "ZActorComponent_Interop.h"

#include "Conjugate/ZConjugateRegistry_UObject.h"
#include "Conjugate/ZConjugateUnsafe.h"
#include "Interop/ZInteropExceptionHelper.h"

ENetMode ZSharp::FZActorComponent_Interop::GetNetMode(FZConjugateHandle self)
{
	TRY
	{
		auto pSelf = ConjugateUnsafe<UActorComponent>(self);
		return pSelf->GetNetMode();
	}
	CATCHR(NM_Standalone)
}


