// Copyright Zero Games. All Rights Reserved.

#include "ZGameplayTag_Interop.h"

#include "GameplayTagContainer.h"
#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZStrangeConjugateRegistries.h"

ZSharp::FZConjugateHandle ZSharp::FZGameplayTag_Interop::Request(const TCHAR* tagName)
{
	return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>().Conjugate(FGameplayTag::StaticStruct(), [tagName](const FZSelfDescriptiveScriptStruct& sdss)
	{
		*sdss.GetTypedUnderlyingInstance<FGameplayTag>() = FGameplayTag::RequestGameplayTag(tagName, false);
	});
}


