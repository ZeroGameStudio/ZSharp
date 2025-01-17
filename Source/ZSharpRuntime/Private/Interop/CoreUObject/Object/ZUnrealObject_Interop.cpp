// Copyright Zero Games. All Rights Reserved.

#include "ZUnrealObject_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"

ZSharp::FZConjugateHandle ZSharp::FZUnrealObject_Interop::GetClass(FZConjugateHandle self)
{
	FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	auto pSelf = registry.ConjugateUnsafeChecked<const UObject>(self);
	return registry.Conjugate(pSelf->GetClass());
}

void ZSharp::FZUnrealObject_Interop::GetName(FZConjugateHandle self, FString& result)
{
	const FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	auto pSelf = registry.ConjugateUnsafeChecked<const UObject>(self);
	result = pSelf->GetName();
}

void ZSharp::FZUnrealObject_Interop::GetPathName(FZConjugateHandle self, FString& result)
{
	const FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	auto pSelf = registry.ConjugateUnsafeChecked<const UObject>(self);
	result = pSelf->GetPathName();
}

ZSharp::FZConjugateHandle ZSharp::FZUnrealObject_Interop::GetOuter(FZConjugateHandle self)
{
	FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	auto pSelf = registry.ConjugateUnsafeChecked<const UObject>(self);
	return registry.Conjugate(pSelf->GetOuter());
}

ZSharp::FZConjugateHandle ZSharp::FZUnrealObject_Interop::GetWorld(FZConjugateHandle self)
{
#if WITH_ENGINE
	FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	auto pSelf = registry.ConjugateUnsafeChecked<const UObject>(self);
	return registry.Conjugate(pSelf->GetWorld());
#else
	return {};
#endif
}

uint8 ZSharp::FZUnrealObject_Interop::IsA(FZConjugateHandle self, FZConjugateHandle cls)
{
	const FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	auto pSelf = registry.ConjugateUnsafeChecked<const UObject>(self);
	auto pCls = registry.ConjugateUnsafeChecked<const UClass>(cls);
	return pSelf->IsA(pCls);
}

uint8 ZSharp::FZUnrealObject_Interop::IsIn(FZConjugateHandle self, FZConjugateHandle outer)
{
	const FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	auto pSelf = registry.ConjugateUnsafeChecked<const UObject>(self);
	auto pOuter = registry.ConjugateUnsafeChecked<const UObject>(outer);
	return pSelf->IsIn(pOuter);
}

uint8 ZSharp::FZUnrealObject_Interop::Rename(FZConjugateHandle self, const TCHAR* newName, FZConjugateHandle newOuter)
{
	const FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	auto pSelf = registry.ConjugateUnsafeChecked<UObject>(self);
	auto pNewOuter = registry.ConjugateUnsafeChecked<UObject>(newOuter);
	return pSelf->Rename(newName, pNewOuter);
}


