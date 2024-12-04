// Copyright Zero Games. All Rights Reserved.

#include "ZUnrealObject_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"

ZSharp::FZConjugateHandle ZSharp::FZUnrealObject_Interop::GetClass(FZConjugateHandle self)
{
	FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	const auto pSelf = registry.ConjugateUnsafeChecked<UObject>(self);
	return registry.Conjugate(pSelf->GetClass());
}

void ZSharp::FZUnrealObject_Interop::GetName(FZConjugateHandle self, FString& result)
{
	const FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	const auto pSelf = registry.ConjugateUnsafeChecked<UObject>(self);
	result = pSelf->GetName();
}

void ZSharp::FZUnrealObject_Interop::GetPathName(FZConjugateHandle self, FString& result)
{
	const FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	const auto pSelf = registry.ConjugateUnsafeChecked<UObject>(self);
	result = pSelf->GetPathName();
}

ZSharp::FZConjugateHandle ZSharp::FZUnrealObject_Interop::GetOuter(FZConjugateHandle self)
{
	FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	const auto pSelf = registry.ConjugateUnsafeChecked<UObject>(self);
	return registry.Conjugate(pSelf->GetOuter());
}

uint8 ZSharp::FZUnrealObject_Interop::IsA(FZConjugateHandle self, FZConjugateHandle cls)
{
	const FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	const auto pSelf = registry.ConjugateUnsafeChecked<UObject>(self);
	const auto pCls = registry.ConjugateUnsafeChecked<UClass>(cls);
	return pSelf->IsA(pCls);
}

uint8 ZSharp::FZUnrealObject_Interop::IsIn(FZConjugateHandle self, FZConjugateHandle outer)
{
	const FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	const auto pSelf = registry.ConjugateUnsafeChecked<UObject>(self);
	const auto pOuter = registry.ConjugateUnsafeChecked<UObject>(outer);
	return pSelf->IsIn(pOuter);
}

uint8 ZSharp::FZUnrealObject_Interop::Rename(FZConjugateHandle self, const TCHAR* newName, FZConjugateHandle newOuter)
{
	const FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	const auto pSelf = registry.ConjugateUnsafeChecked<UObject>(self);
	const auto pNewOuter = registry.ConjugateUnsafeChecked<UObject>(newOuter);
	return pSelf->Rename(newName, pNewOuter);
}


