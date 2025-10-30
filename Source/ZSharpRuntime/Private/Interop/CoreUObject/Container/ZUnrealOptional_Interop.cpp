// Copyright Zero Games. All Rights Reserved.

#include "ZUnrealOptional_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZStrangeConjugateRegistries.h"
#include "Interop/ZInteropExceptionHelper.h"
#include "Reflection/Wrapper/ZSelfDescriptiveOptional.h"

uint8 ZSharp::FZUnrealOptional_Interop::Get(FZConjugateHandle self, FZCallBufferSlot& value)
{
	TRY
	{
		FZSelfDescriptiveOptional* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Optional>().ConjugateUnsafe(self);
		return sdself->Get(value);
	}
	CATCHR(false)
}

void ZSharp::FZUnrealOptional_Interop::Set(FZConjugateHandle self, FZCallBufferSlot value)
{
	GUARD
	(
		FZSelfDescriptiveOptional* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Optional>().ConjugateUnsafe(self);
		sdself->Set(value);
	);
}

void ZSharp::FZUnrealOptional_Interop::Reset(FZConjugateHandle self)
{
	GUARD
	(
		FZSelfDescriptiveOptional* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Optional>().ConjugateUnsafe(self);
		sdself->Reset();
	);
}

uint8 ZSharp::FZUnrealOptional_Interop::IsSet(FZConjugateHandle self)
{
	TRY
	{
		FZSelfDescriptiveOptional* sdself = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Optional>().ConjugateUnsafe(self);
		return sdself->IsSet();
	}
	CATCHR(false)
}


