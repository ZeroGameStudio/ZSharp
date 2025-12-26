// Copyright Zero Games. All Rights Reserved.

#include "ZUnrealName_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZRegularConjugateRegistries.h"
#include "Interop/ZInteropExceptionHelper.h"

namespace ZSharp::ZUnrealName_Interop_Private
{
	static void GetData(FZConjugateHandle self, FString& result)
	{
		const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Name>();
		const FName* nameseslf = registry.ConjugateUnsafe(self);
		result = nameseslf->ToString();
	}
}

void ZSharp::FZUnrealName_Interop::GetData(FZConjugateHandle self, FString& result)
{
	GUARD(ZUnrealName_Interop_Private::GetData(self, result));
}

void ZSharp::FZUnrealName_Interop::SetData(FZConjugateHandle self, const TCHAR* data)
{
	GUARD
	(
		const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Name>();
		FName* nameself = registry.ConjugateUnsafe(self);
		*nameself = data;
	);
}

uint8 ZSharp::FZUnrealName_Interop::IsNone(FZConjugateHandle self)
{
	TRY
	{
		const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Name>();
		FName* nameself = registry.ConjugateUnsafe(self);
		return nameself->IsNone();
	}
	CATCHR(false)
}


