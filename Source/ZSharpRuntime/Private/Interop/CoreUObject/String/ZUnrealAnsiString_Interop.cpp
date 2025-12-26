// Copyright Zero Games. All Rights Reserved.

#include "ZUnrealAnsiString_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZRegularConjugateRegistries.h"
#include "Interop/ZInteropExceptionHelper.h"

namespace ZSharp::ZUnrealAnsiString_Interop_Private
{
	static void GetData(FZConjugateHandle self, FString& result)
	{
		const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_AnsiString>();
		const FAnsiString* strself = registry.ConjugateUnsafe(self);
		result = **strself;
	}

	static void SetData(FZConjugateHandle self, const TCHAR* data)
	{
		const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_AnsiString>();
		FAnsiString* strself = registry.ConjugateUnsafe(self);
		*strself = data;
	}
}

void ZSharp::FZUnrealAnsiString_Interop::GetData(FZConjugateHandle self, FString& result)
{
	GUARD(ZUnrealAnsiString_Interop_Private::GetData(self, result));
}

void ZSharp::FZUnrealAnsiString_Interop::SetData(FZConjugateHandle self, const TCHAR* data)
{
	GUARD(ZUnrealAnsiString_Interop_Private::SetData(self, data));
}


