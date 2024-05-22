// Copyright Zero Games. All Rights Reserved.


#include "ZCall/Conjugate/ZConjugateRegistry_UObject.h"

#include "ZSharpExportHelpers.h"
#include "ZSharpExportRuntimeSettings.h"
#include "ALC/IZMasterAssemblyLoadContext.h"
#include "ZCall/ZDeclareConjugateRegistry.h"

namespace ZSharp::ZConjugateRegistry_UObject_Private
{
	static TZDeclareConjugateRegistry<FZConjugateRegistry_UObject> GDeclare;
}

ZSharp::FZConjugateRegistry_UObject::FZConjugateRegistry_UObject(IZMasterAssemblyLoadContext& alc)
	: Super(alc)
{
	GCDelegate = FCoreUObjectDelegates::GarbageCollectComplete.AddRaw(this, &ThisClass::HandleGarbageCollectComplete);
}

ZSharp::FZConjugateRegistry_UObject::~FZConjugateRegistry_UObject()
{
	FCoreUObjectDelegates::GarbageCollectComplete.Remove(GCDelegate);
}

ZSharp::FZRuntimeTypeHandle ZSharp::FZConjugateRegistry_UObject::GetManagedType(const ConjugateType* unmanaged) const
{
	const UClass* cls = unmanaged->GetClass();
	const FString moduleName = FZSharpExportHelpers::GetUFieldModuleName(cls);
	FString assemblyName;
	if (!GetDefault<UZSharpExportRuntimeSettings>()->TryGetModuleAssembly(moduleName, assemblyName))
	{
		assemblyName = ZSHARP_ENGINE_ASSEMBLY_NAME;
	}
	const FString outerExportName = FZSharpExportHelpers::GetUFieldOuterExportName(cls);
	const FString typeName = FString::Printf(TEXT("%s.%s"), *assemblyName, *outerExportName);
	
	return Alc.GetType(assemblyName, typeName);
}

ZSharp::FZConjugateRegistry_UObject::ConjugateType* ZSharp::FZConjugateRegistry_UObject::GetUnmanaged(const RecordType* rec) const
{
	return rec->TypedUnmanaged.ResolveObjectPtrEvenIfGarbage();
}

ZSharp::FZConjugateRegistry_UObject::RecordType ZSharp::FZConjugateRegistry_UObject::BuildRedConjugateRec(ConjugateType* unmanaged, bool bOwning)
{
	check(!bOwning);
	return { unmanaged };
}

ZSharp::FZConjugateRegistry_UObject::ConjugateType* ZSharp::FZConjugateRegistry_UObject::BuildBlackConjugateRec()
{
	// There is no black UObject conjugate.
	checkNoEntry();
	return {};
}

void ZSharp::FZConjugateRegistry_UObject::InternalReleaseConjugate(void* unmanaged, const RecordType* rec)
{
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


