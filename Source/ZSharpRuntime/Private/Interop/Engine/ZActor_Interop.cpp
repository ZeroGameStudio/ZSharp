// Copyright Zero Games. All Rights Reserved.

#include "ZActor_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"

void ZSharp::FZActor_Interop::FinishSpawning(FZConjugateHandle self)
{
	auto pSelf = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe<AActor>(self);
	pSelf->FinishSpawning(FTransform::Identity, true);
}

ENetMode ZSharp::FZActor_Interop::GetNetMode(FZConjugateHandle self)
{
	auto pSelf = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe<AActor>(self);
	return pSelf->GetNetMode();
}


