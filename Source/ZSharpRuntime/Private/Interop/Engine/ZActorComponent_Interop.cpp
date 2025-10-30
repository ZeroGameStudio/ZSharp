// Copyright Zero Games. All Rights Reserved.

#include "ZActorComponent_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"
#include "Interop/ZInteropExceptionHelper.h"

ENetMode ZSharp::FZActorComponent_Interop::GetNetMode(FZConjugateHandle self)
{
	TRY
	{
		auto pSelf = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe<UActorComponent>(self);
		return pSelf->GetNetMode();
	}
	CATCHR(NM_Standalone)
}


