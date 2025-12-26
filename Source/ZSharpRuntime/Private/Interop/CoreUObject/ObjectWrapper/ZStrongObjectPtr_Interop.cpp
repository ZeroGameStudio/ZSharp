// Copyright Zero Games. All Rights Reserved.

#include "ZStrongObjectPtr_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"
#include "Interop/ZInteropExceptionHelper.h"

namespace ZSharp::ZStrongObjectPtr_Interop_Private
{
	static FZStrongObjectPtr_Interop::FStrongObjectPtr* Alloc(FZConjugateHandle target)
	{
		return new FZStrongObjectPtr_Interop::FStrongObjectPtr { IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe(target) };
	}
}

ZSharp::FZStrongObjectPtr_Interop::FStrongObjectPtr* ZSharp::FZStrongObjectPtr_Interop::Alloc(FZConjugateHandle target)
{
	TRY
	{
		return ZStrongObjectPtr_Interop_Private::Alloc(target);
	}
	CATCHR(nullptr)
}

void ZSharp::FZStrongObjectPtr_Interop::Free(FStrongObjectPtr* self)
{
	GUARD(delete self);
}

uint8 ZSharp::FZStrongObjectPtr_Interop::Identical(FStrongObjectPtr* self, FStrongObjectPtr* other)
{
	TRY
	{
		return *self == *other;
	}
	CATCHR(false)
}

int32 ZSharp::FZStrongObjectPtr_Interop::Hash(FStrongObjectPtr* self)
{
	TRY
	{
		return GetTypeHash(*self);
	}
	CATCHR(0)
}

ZSharp::FZConjugateHandle ZSharp::FZStrongObjectPtr_Interop::Get(FStrongObjectPtr* self, uint8 evenIfGarbage)
{
	TRY
	{
		UObject* result = IsValid(self, evenIfGarbage) ? self->Get() : nullptr;
		return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().Conjugate(result);
	}
	CATCHR({})
}

void ZSharp::FZStrongObjectPtr_Interop::Set(FStrongObjectPtr* self, FZConjugateHandle target)
{
	GUARD(self->Reset(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe(target)));
}

uint8 ZSharp::FZStrongObjectPtr_Interop::IsValid(FStrongObjectPtr* self, uint8 evenIfGarbage)
{
	TRY
	{
		return !!evenIfGarbage || ::IsValid(self->Get());
	}
	CATCHR(false)
}

uint8 ZSharp::FZStrongObjectPtr_Interop::IsNull(FStrongObjectPtr* self)
{
	TRY
	{
		return !self->Get();
	}
	CATCHR(false)
}
