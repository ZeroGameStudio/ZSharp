// Copyright Zero Games. All Rights Reserved.

#include "ZUnrealStruct_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"

uint8 ZSharp::FZUnrealStruct_Interop::IsChildOf(FZConjugateHandle self, FZConjugateHandle other)
{
	const FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	auto pSelf = registry.ConjugateUnsafeChecked<const UStruct>(self);
	auto pOther = registry.ConjugateUnsafeChecked<const UStruct>(other);
	return pSelf->IsChildOf(pOther);
}


