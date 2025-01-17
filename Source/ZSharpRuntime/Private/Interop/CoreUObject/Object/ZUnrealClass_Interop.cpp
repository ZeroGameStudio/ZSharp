// Copyright Zero Games. All Rights Reserved.

#include "ZUnrealClass_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"

ZSharp::FZConjugateHandle ZSharp::FZUnrealClass_Interop::GetDefaultObject(FZConjugateHandle self, uint8 createIfNeeded)
{
	FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	auto pSelf = registry.ConjugateUnsafeChecked<const UClass>(self);
	return registry.Conjugate(pSelf->GetDefaultObject(!!createIfNeeded));
}

uint8 ZSharp::FZUnrealClass_Interop::IsInterface(FZConjugateHandle self)
{
	const FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	auto pSelf = registry.ConjugateUnsafeChecked<const UClass>(self);
	return pSelf->HasAllClassFlags(CLASS_Interface);
}

uint8 ZSharp::FZUnrealClass_Interop::ImplementsInterface(FZConjugateHandle self, FZConjugateHandle interface)
{
	const FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	auto pSelf = registry.ConjugateUnsafeChecked<const UClass>(self);
	auto pInterface = registry.ConjugateUnsafeChecked<const UClass>(interface);
	return pSelf->ImplementsInterface(pInterface);
}


