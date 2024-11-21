// Copyright Zero Games. All Rights Reserved.

#include "WeakObjectPtr_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"
#include "Conjugate/ZObjectWrapperConjugateRegisties.h"
#include "Reflection/Wrapper/ZSelfDescriptiveObjectWrappers.h"

void ZSharp::FZWeakObjectPtr_Interop::Copy(FZConjugateHandle self, FZConjugateHandle other)
{
	const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_WeakObjectPtr>();
	FZSelfDescriptiveWeakObjectPtr* sdself = registry.ConjugateUnsafe(self);
	FZSelfDescriptiveWeakObjectPtr* sdother = registry.ConjugateUnsafe(other);
	*sdself->GetUnderlyingInstance() = sdother && sdother->GetDescriptor()->IsChildOf(sdself->GetDescriptor()) ? *sdother->GetUnderlyingInstance() : nullptr;
}

uint8 ZSharp::FZWeakObjectPtr_Interop::Equals(FZConjugateHandle self, FZConjugateHandle other)
{
	const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_WeakObjectPtr>();
	FZSelfDescriptiveWeakObjectPtr* sdself = registry.ConjugateUnsafe(self);
	FZSelfDescriptiveWeakObjectPtr* sdother = registry.ConjugateUnsafe(other);
	return sdself == sdother || *sdself->GetUnderlyingInstance() == *sdother->GetUnderlyingInstance();
}

int32 ZSharp::FZWeakObjectPtr_Interop::GetHashCode(FZConjugateHandle self)
{
	const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_WeakObjectPtr>();
	FZSelfDescriptiveWeakObjectPtr* sdself = registry.ConjugateUnsafe(self);
	return GetTypeHash(sdself->GetUnderlyingInstance());
}

ZSharp::FZConjugateHandle ZSharp::FZWeakObjectPtr_Interop::Get(FZConjugateHandle self, uint8 eventIfGarbage)
{
	const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_WeakObjectPtr>();
	FZSelfDescriptiveWeakObjectPtr* sdself = registry.ConjugateUnsafe(self);
	const UObject* obj = sdself->GetUnderlyingInstance()->Get(!!eventIfGarbage);
	if (!obj)
	{
		return {};
	}
	
	return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().Conjugate(obj);
}

void ZSharp::FZWeakObjectPtr_Interop::Set(FZConjugateHandle self, FZConjugateHandle obj)
{
	const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_WeakObjectPtr>();
	FZSelfDescriptiveWeakObjectPtr* sdself = registry.ConjugateUnsafe(self);
	sdself->Set(obj ? IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe(obj) : nullptr);
}

uint8 ZSharp::FZWeakObjectPtr_Interop::IsValid(FZConjugateHandle self, uint8 eventIfGarbage)
{
	const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_WeakObjectPtr>();
	FZSelfDescriptiveWeakObjectPtr* sdself = registry.ConjugateUnsafe(self);
	return sdself->GetUnderlyingInstance()->IsValid(!!eventIfGarbage);
}

uint8 ZSharp::FZWeakObjectPtr_Interop::IsNull(FZConjugateHandle self)
{
	const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_WeakObjectPtr>();
	FZSelfDescriptiveWeakObjectPtr* sdself = registry.ConjugateUnsafe(self);
	return sdself->GetUnderlyingInstance()->IsExplicitlyNull();
}

uint8 ZSharp::FZWeakObjectPtr_Interop::IsStale(FZConjugateHandle self, uint8 includingGarbage)
{
	const auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_WeakObjectPtr>();
	FZSelfDescriptiveWeakObjectPtr* sdself = registry.ConjugateUnsafe(self);
	return sdself->GetUnderlyingInstance()->IsStale(!!includingGarbage);
}


