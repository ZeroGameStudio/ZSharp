// Copyright Zero Games. All Rights Reserved.

#include "ZGameplayTag_Interop.h"

#include "GameplayTagContainer.h"
#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZStrangeConjugateRegistries.h"
#include "Interop/ZInteropExceptionHelper.h"

namespace ZSharp::ZGameplayTag_Interop_Private
{
	static FZConjugateHandle Request(const TCHAR* tagName)
	{
		return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>().Conjugate(FGameplayTag::StaticStruct(), [tagName](const FZSelfDescriptiveScriptStruct& sdss)
		{
			*sdss.GetTypedUnderlyingInstance<FGameplayTag>() = FGameplayTag::RequestGameplayTag(tagName, false);
		});
	}
}

ZSharp::FZConjugateHandle ZSharp::FZGameplayTag_Interop::Request(const TCHAR* tagName)
{
	TRY
	{
		return ZGameplayTag_Interop_Private::Request(tagName);
	}
	CATCHR({})
}


