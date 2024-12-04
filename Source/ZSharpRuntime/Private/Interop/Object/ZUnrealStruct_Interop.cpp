// Copyright Zero Games. All Rights Reserved.

#include "ZUnrealStruct_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"

uint8 ZSharp::FZUnrealStruct_Interop::IsChildOf(FZConjugateHandle self, FZConjugateHandle other)
{
	const FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	const auto pSelf = registry.ConjugateUnsafeChecked<UStruct>(self);
	const auto pOther = registry.ConjugateUnsafeChecked<UStruct>(other);
	return pSelf->IsChildOf(pOther);
}


