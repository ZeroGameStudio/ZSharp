// Copyright Zero Games. All Rights Reserved.

#include "ZWorld_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"
#include "Conjugate/ZStrangeConjugateRegistries.h"

ZSharp::FZConjugateHandle ZSharp::FZWorld_Interop::SpawnActor(FZConjugateHandle self, FZConjugateHandle cls, FZConjugateHandle transform, const TCHAR* name, FZConjugateHandle actorTemplate, FZConjugateHandle owner, FZConjugateHandle instigator, FZConjugateHandle overrideLevel, ESpawnActorCollisionHandlingMethod spawnCollisionHandlingOverride, ESpawnActorScaleMethod transformScaleMethod, FActorSpawnParameters::ESpawnActorNameMode nameMode, uint8 absolute, uint8 deferred)
{
	IZMasterAssemblyLoadContext* alc = IZSharpClr::Get().GetMasterAlc();
	FZConjugateRegistry_UObject& registry = alc->GetConjugateRegistry<FZConjugateRegistry_UObject>();

	UWorld* pSelf = registry.ConjugateUnsafe<UWorld>(self);
	UClass* pCls = registry.ConjugateUnsafe<UClass>(cls);
	FZSelfDescriptiveScriptStruct* sdTransform = alc->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>().ConjugateUnsafe(transform);
	const FTransform& spawnTransform = sdTransform->GetDescriptor() == TBaseStructure<FTransform>::Get() ? *static_cast<FTransform*>(sdTransform->GetUnderlyingInstance()) : FTransform::Identity;
	AActor* pTemplate = registry.ConjugateUnsafe<AActor>(actorTemplate);
	AActor* pOwner = registry.ConjugateUnsafe<AActor>(owner);
	APawn* pInstigator = registry.ConjugateUnsafe<APawn>(instigator);
	ULevel* pOverrideLevel = registry.ConjugateUnsafe<ULevel>(overrideLevel);

	FActorSpawnParameters params;
	params.Name = name;
	params.Template = pTemplate;
	params.Owner = pOwner;
	params.Instigator = pInstigator;
	params.OverrideLevel = pOverrideLevel;
	params.SpawnCollisionHandlingOverride = spawnCollisionHandlingOverride;
	params.TransformScaleMethod = transformScaleMethod;
	params.NameMode = nameMode;
	params.bDeferConstruction = !!deferred;

	return registry.Conjugate(absolute ? pSelf->SpawnActorAbsolute(pCls, spawnTransform, params) : pSelf->SpawnActor(pCls, &spawnTransform, params));
}


