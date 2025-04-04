﻿// Copyright Zero Games. All Rights Reserved.

#include "ZUnrealString_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZRegularConjugateRegistries.h"

const TCHAR* ZSharp::FZUnrealString_Interop::GetData(FZConjugateHandle self)
{
	const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_String>();
	const FString* strself = registry.ConjugateUnsafe(self);
	return **strself;
}

void ZSharp::FZUnrealString_Interop::SetData(FZConjugateHandle self, const TCHAR* data)
{
	const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_String>();
	FString* strself = registry.ConjugateUnsafe(self);
	*strself = data;
}


