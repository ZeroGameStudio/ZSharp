// Copyright Zero Games. All Rights Reserved.

#include "ZUnrealText_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZRegularConjugateRegistries.h"
#include "Interop/ZInteropExceptionHelper.h"

namespace ZSharp::ZUnrealText_Interop_Private
{
	static void SetData(FZConjugateHandle self, const TCHAR* data)
	{
		const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Text>();
		FText* textself = registry.ConjugateUnsafe(self);
		*textself = FText::FromString(data);
	}
}

const TCHAR* ZSharp::FZUnrealText_Interop::GetData(FZConjugateHandle self)
{
	TRY
	{
		const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Text>();
		const FText* textself = registry.ConjugateUnsafe(self);
		return *textself->ToString();
	}
	CATCHR(nullptr)
}

void ZSharp::FZUnrealText_Interop::SetData(FZConjugateHandle self, const TCHAR* data)
{
	GUARD(ZUnrealText_Interop_Private::SetData(self, data));
}


