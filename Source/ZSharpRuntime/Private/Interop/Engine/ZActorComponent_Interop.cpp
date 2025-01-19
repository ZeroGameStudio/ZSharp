// Copyright Zero Games. All Rights Reserved.

#include "ZActorComponent_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"

ENetMode ZSharp::FZActorComponent_Interop::GetNetMode(FZConjugateHandle self)
{
	auto pSelf = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe<UActorComponent>(self);
	return pSelf->GetNetMode();
}
