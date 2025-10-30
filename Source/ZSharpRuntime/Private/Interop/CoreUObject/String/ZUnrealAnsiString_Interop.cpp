// Copyright Zero Games. All Rights Reserved.

#include "ZUnrealAnsiString_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZRegularConjugateRegistries.h"
#include "Interop/ZInteropExceptionHelper.h"

void ZSharp::FZUnrealAnsiString_Interop::GetData(FZConjugateHandle self, FString& result)
{
	GUARD
	(
		const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_AnsiString>();
		const FAnsiString* strself = registry.ConjugateUnsafe(self);
		result = **strself;
	);
}

void ZSharp::FZUnrealAnsiString_Interop::SetData(FZConjugateHandle self, const TCHAR* data)
{
	GUARD
	(
		const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_AnsiString>();
		FAnsiString* strself = registry.ConjugateUnsafe(self);
		*strself = data;
	);
}


