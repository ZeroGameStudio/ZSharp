// Copyright Zero Games. All Rights Reserved.


#include "ZCall/Conjugate/ZConjugateRegistry_UObject.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "ZCall/ZDeclareConjugateRegistry.h"

namespace ZSharp::ZConjugateRegistry_UObject_Private
{
	static TZDeclareConjugateRegistry<FZConjugateRegistry_UObject> GDeclare;
}

ZSharp::FZConjugateRegistry_UObject::FZConjugateRegistry_UObject(IZMasterAssemblyLoadContext& alc)
	: Alc(alc)
{
	GCDelegate = FCoreUObjectDelegates::GarbageCollectComplete.AddRaw(this, &ThisClass::HandleGarbageCollectComplete);
}

ZSharp::FZConjugateRegistry_UObject::~FZConjugateRegistry_UObject()
{
	FCoreUObjectDelegates::GarbageCollectComplete.Remove(GCDelegate);
}

UObject* ZSharp::FZConjugateRegistry_UObject::Conjugate(FZConjugateHandle handle) const
{
	void* unmanaged = handle.Handle;
	const FZRec* rec = ConjugateMap.Find(unmanaged);
	return rec ? rec->TypedUnmanaged.ResolveObjectPtrEvenIfGarbage() : nullptr;
}

ZSharp::FZConjugateHandle ZSharp::FZConjugateRegistry_UObject::Conjugate(const UObjectBase* unmanaged)
{
	UObject* typedUnmanaged = StaticCast<UObject*>(const_cast<UObjectBase*>(unmanaged));
	FZRec* rec = ConjugateMap.Find(typedUnmanaged);
	if (rec)
	{
		return { rec->TypedUnmanaged.ResolveObjectPtrEvenIfGarbage() };
	}
	else
	{
		ConjugateMap.Emplace(typedUnmanaged, { typedUnmanaged });
		FZRuntimeTypeHandle type = Alc.GetType("ZeroGames.ZSharp.UnrealEngine", "ZeroGames.ZSharp.UnrealEngine.CoreUObject.UnrealObject");
		Alc.BuildConjugate(typedUnmanaged, type);
		return { typedUnmanaged };
	}
}

void ZSharp::FZConjugateRegistry_UObject::Release()
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

void* ZSharp::FZConjugateRegistry_UObject::BuildConjugate()
{
	// There is no black UObject conjugate.
	checkNoEntry();
	return nullptr;
}

void ZSharp::FZConjugateRegistry_UObject::ReleaseConjugate(void* unmanaged)
{
	// There is no black UObject conjugate.
	checkNoEntry();
}

void ZSharp::FZConjugateRegistry_UObject::PushRedFrame()
{
	RedStack.Push({});
}

void ZSharp::FZConjugateRegistry_UObject::PopRedFrame()
{
	FZRedFrame frame = RedStack.Pop();
	for (const auto& conjugate : frame.CapturedConjugates)
	{
		InternalReleaseConjugate(conjugate);
	}
}

void ZSharp::FZConjugateRegistry_UObject::InternalReleaseConjugate(void* unmanaged)
{
	const FZRec* rec = ConjugateMap.Find(unmanaged);
	if (!rec)
	{
		return;
	}

	Alc.ReleaseConjugate(unmanaged);

	ConjugateMap.Remove(unmanaged);
}

void ZSharp::FZConjugateRegistry_UObject::HandleGarbageCollectComplete()
{
	TArray<void*> pendingRemoves;
	for (const auto& pair : ConjugateMap)
	{
		if (!pair.Value.TypedUnmanaged.ResolveObjectPtrEvenIfGarbage())
		{
			pendingRemoves.Emplace(pair.Key);
		}
	}

	for (const auto& key : pendingRemoves)
	{
		ConjugateMap.Remove(key);
	}
}


