// Copyright Zero Games. All Rights Reserved.

#include "ZUnrealObjectGlobals_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"

ZSharp::FZConjugateHandle ZSharp::FZUnrealObjectGlobals_Interop::NewObject(FZConjugateHandle cls, FZConjugateHandle outer, const TCHAR* name)
{
	FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	auto pCls = registry.ConjugateUnsafeChecked<const UClass>(cls);
	auto pOuter = registry.ConjugateUnsafe<UObject>(outer);
	if (!pOuter)
	{
		pOuter = ::GetTransientPackage();
	}
	FStaticConstructObjectParameters params { pCls };
	params.Outer = pOuter;
	params.Name = name;
	return registry.Conjugate(StaticConstructObject_Internal(params));
}

ZSharp::FZConjugateHandle ZSharp::FZUnrealObjectGlobals_Interop::LoadObject(FZConjugateHandle cls, FZConjugateHandle outer, const TCHAR* name)
{
	FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	auto pCls = registry.ConjugateUnsafeChecked<UClass>(cls);
	auto pOuter = registry.ConjugateUnsafe<UObject>(outer);
	return registry.Conjugate(StaticLoadObject(pCls, pOuter, name));
}

ZSharp::FZConjugateHandle ZSharp::FZUnrealObjectGlobals_Interop::DuplicateObject(FZConjugateHandle source, FZConjugateHandle outer, const TCHAR* name, FZConjugateHandle cls)
{
	FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	auto pSource = registry.ConjugateUnsafeChecked<const UObject>(source);
	auto pOuter = registry.ConjugateUnsafeChecked<UObject>(outer);
	auto pCls = registry.ConjugateUnsafe<UClass>(cls);
	return registry.Conjugate(StaticDuplicateObject(pSource, pOuter, name, RF_AllFlags, pCls));
}

ZSharp::FZConjugateHandle ZSharp::FZUnrealObjectGlobals_Interop::FindObject(FZConjugateHandle cls, FZConjugateHandle outer, const TCHAR* name, uint8 exactClass)
{
	FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	auto pCls = registry.ConjugateUnsafeChecked<UClass>(cls);
	auto pOuter = registry.ConjugateUnsafe<UObject>(outer);
	return registry.Conjugate(StaticFindObject(pCls, pOuter, name, !!exactClass));
}

ZSharp::FZConjugateHandle ZSharp::FZUnrealObjectGlobals_Interop::LowLevelFindObject(const TCHAR* path)
{
	FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	return registry.Conjugate(StaticFindObject(UObject::StaticClass(), nullptr, path));
}

ZSharp::FZConjugateHandle ZSharp::FZUnrealObjectGlobals_Interop::GetTransientPackage()
{
	FZConjugateRegistry_UObject& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	return registry.Conjugate(::GetTransientPackage());
}


