// Copyright Zero Games. All Rights Reserved.


#include "ZCall/Conjugate/ZConjugateRegistry_String.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "ZCall/ZDeclareConjugateRegistry.h"

namespace ZSharp::ZConjugateRegistry_UObject_Private
{
	static TZDeclareConjugateRegistry<FZConjugateRegistry_String> GDeclare;
}

ZSharp::FZConjugateRegistry_String::FZConjugateRegistry_String(IZMasterAssemblyLoadContext& alc)
	: Alc(alc)
{
}

FString* ZSharp::FZConjugateRegistry_String::Conjugate(FZConjugateHandle handle) const
{
	void* unmanaged = handle.Handle;
	const FZRec* rec = ConjugateMap.Find(unmanaged);
	return rec ? rec->TypedUnmanaged : nullptr;
}

ZSharp::FZConjugateHandle ZSharp::FZConjugateRegistry_String::Conjugate(FString* unmanaged, bool bOwning)
{
	FZRec* rec = ConjugateMap.Find(unmanaged);
	if (rec)
	{
		// This branch means conjugating an unmanaged object that doesn't owned by the caller.
		check(!bOwning);
		return { rec->TypedUnmanaged };
	}
	else
	{
		ConjugateMap.Emplace(unmanaged, { unmanaged, bOwning, false });
		FZRuntimeTypeHandle type = Alc.GetType("ZeroGames.ZSharp.UnrealEngine", "ZeroGames.ZSharp.UnrealEngine.Core.UnrealString");
		Alc.BuildConjugate(unmanaged, type);
		return { unmanaged };
	}
}

void ZSharp::FZConjugateRegistry_String::Release()
{
	TArray<void*> conjugates;
	for (const auto& pair : ConjugateMap)
	{
		conjugates.Emplace(pair.Key);
	}

	for (const auto& conjugate : conjugates)
	{
		InternalReleaseConjugate(conjugate);
	}
}

void* ZSharp::FZConjugateRegistry_String::BuildConjugate()
{
	auto conjugate = new FString;
	ConjugateMap.Emplace(conjugate, { conjugate, true, true });
	return conjugate;
}

void ZSharp::FZConjugateRegistry_String::ReleaseConjugate(void* unmanaged)
{
	InternalReleaseConjugate(unmanaged);
}

void ZSharp::FZConjugateRegistry_String::PushRedFrame()
{
	RedStack.Push({});
}

void ZSharp::FZConjugateRegistry_String::PopRedFrame()
{
	FZRedFrame frame = RedStack.Pop();
	for (const auto& conjugate : frame.CapturedConjugates)
	{
		InternalReleaseConjugate(conjugate);
	}
}

void ZSharp::FZConjugateRegistry_String::InternalReleaseConjugate(void* unmanaged)
{
	const FZRec* rec = ConjugateMap.Find(unmanaged);
	if (!rec)
	{
		return;
	}

	if (rec->bOwning)
	{
		delete rec->TypedUnmanaged;
	}

	if (!rec->bBlack)
	{
		Alc.ReleaseConjugate(unmanaged);
	}

	ConjugateMap.Remove(unmanaged);
}


