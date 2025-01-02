// Copyright Zero Games. All Rights Reserved.

#include "ZStrongObjectPtr_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"

ZSharp::FZStrongObjectPtr_Interop::FStrongObjectPtr* ZSharp::FZStrongObjectPtr_Interop::Alloc(FZConjugateHandle target)
{
	return new FStrongObjectPtr { IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe(target) };
}

void ZSharp::FZStrongObjectPtr_Interop::Free(FStrongObjectPtr* self)
{
	delete self;
}

uint8 ZSharp::FZStrongObjectPtr_Interop::Identical(FStrongObjectPtr* self, FStrongObjectPtr* other)
{
	return *self == *other;
}

int32 ZSharp::FZStrongObjectPtr_Interop::Hash(FStrongObjectPtr* self)
{
	return GetTypeHash(*self);
}

ZSharp::FZConjugateHandle ZSharp::FZStrongObjectPtr_Interop::Get(FStrongObjectPtr* self, uint8 evenIfGarbage)
{
	UObject* result = IsValid(self, evenIfGarbage) ? self->Get() : nullptr;
	return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().Conjugate(result);
}

void ZSharp::FZStrongObjectPtr_Interop::Set(FStrongObjectPtr* self, FZConjugateHandle target)
{
	self->Reset(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe(target));
}

uint8 ZSharp::FZStrongObjectPtr_Interop::IsValid(FStrongObjectPtr* self, uint8 evenIfGarbage)
{
	return !!evenIfGarbage || ::IsValid(self->Get());
}

uint8 ZSharp::FZStrongObjectPtr_Interop::IsNull(FStrongObjectPtr* self)
{
	return !self->Get();
}
