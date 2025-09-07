// Copyright Zero Games. All Rights Reserved.

#include "ZEngine_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"

uint8 ZSharp::FZEngine_Interop::IsInitialized()
{
	return GEngine && GEngine->IsInitialized();
}

ZSharp::FZConjugateHandle ZSharp::FZEngine_Interop::GetEngine()
{
	return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().Conjugate(GEngine);
}


