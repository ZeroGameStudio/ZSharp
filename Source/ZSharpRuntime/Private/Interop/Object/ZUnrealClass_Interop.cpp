// Copyright Zero Games. All Rights Reserved.

#include "ZUnrealClass_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"

ZSharp::FZConjugateHandle ZSharp::FZUnrealClass_Interop::GetDefaultObject(FZConjugateHandle self, uint8 createIfNeeded)
{
	FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	const auto pSelf = registry.ConjugateUnsafeChecked<UClass>(self);
	return registry.Conjugate(pSelf->GetDefaultObject(!!createIfNeeded));
}

uint8 ZSharp::FZUnrealClass_Interop::IsInterface(FZConjugateHandle self)
{
	const FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	const auto pSelf = registry.ConjugateUnsafeChecked<UClass>(self);
	return pSelf->HasAllClassFlags(CLASS_Interface);
}

uint8 ZSharp::FZUnrealClass_Interop::ImplementsInterface(FZConjugateHandle self, FZConjugateHandle interface)
{
	const FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	const auto pSelf = registry.ConjugateUnsafeChecked<UClass>(self);
	const auto pInterface = registry.ConjugateUnsafeChecked<UClass>(interface);
	return pSelf->ImplementsInterface(pInterface);
}


