// Copyright Zero Games. All Rights Reserved.

#include "ZEngine_Interop.h"

#include "ZSharpRuntimeModule.h"
#include "ALC/IZMasterAssemblyLoadContext.h"
#include "ALC/ZRedFrameScope.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"
#include "Interop/ZInteropExceptionHelper.h"

namespace ZSharp::ZEngine_Interop_Private
{
	struct FZEngineDelegateListener
	{
		FZEngineDelegateListener()
		{
			FCoreDelegates::OnPostEngineInit.AddLambda([]
			{
#if WITH_EDITOR
				if (IZSharpRuntimeModule::Get().HasEngineAssembly())
#endif
				{
					FZRedFrameScope scope;
					FZEngine_Interop::GPostEngineInit();
				}
			
			});
		}
	} GEngineDelegateListener;
}

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


