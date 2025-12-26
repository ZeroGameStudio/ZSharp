// Copyright Zero Games. All Rights Reserved.

#include "ZUnrealUtf8String_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZRegularConjugateRegistries.h"
#include "Interop/ZInteropExceptionHelper.h"

namespace ZSharp::ZUnrealUtf8String_Interop_Private
{
	static void GetData(FZConjugateHandle self, FString& result)
	{
		const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Utf8String>();
		const FUtf8String* strself = registry.ConjugateUnsafe(self);
		result = **strself;
	}

	static void SetData(FZConjugateHandle self, const TCHAR* data)
	{
		const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Utf8String>();
		FUtf8String* strself = registry.ConjugateUnsafe(self);
		*strself = data;
	}
}

void ZSharp::FZUnrealUtf8String_Interop::GetData(FZConjugateHandle self, FString& result)
{
	GUARD(ZUnrealUtf8String_Interop_Private::GetData(self, result));
}

void ZSharp::FZUnrealUtf8String_Interop::SetData(FZConjugateHandle self, const TCHAR* data)
{
	GUARD(ZUnrealUtf8String_Interop_Private::SetData(self, data));
}


