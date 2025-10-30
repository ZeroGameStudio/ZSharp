// Copyright Zero Games. All Rights Reserved.

#include "ZEngine_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"
#include "Interop/ZInteropExceptionHelper.h"

uint8 ZSharp::FZEngine_Interop::IsInitialized()
{
	TRY
	{
		return GEngine && GEngine->IsInitialized();
	}
	CATCHR(false)
}

ZSharp::FZConjugateHandle ZSharp::FZEngine_Interop::GetEngine()
{
	TRY
	{
		return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().Conjugate(GEngine);
	}
	CATCHR({})
}


