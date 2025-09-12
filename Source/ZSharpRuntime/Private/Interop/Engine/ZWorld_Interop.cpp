// Copyright Zero Games. All Rights Reserved.

#include "ZWorld_Interop.h"

#include "ZSharpRuntimeModule.h"
#include "ALC/IZMasterAssemblyLoadContext.h"
#include "ALC/ZRedFrameScope.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"
#include "Conjugate/ZStrangeConjugateRegistries.h"

namespace ZSharp::ZWorld_Interop_Private
{
	struct FWorldDelegateListener
	{
		FWorldDelegateListener()
		{
			FWorldDelegates::OnPostWorldInitialization.AddLambda([](UWorld* world, const UWorld::InitializationValues)
			{
#if WITH_EDITOR
				if (IZSharpRuntimeModule::Get().HasEngineAssembly())
#endif
				{
					FZRedFrameScope scope;
					FZWorld_Interop::GNotifyWorldInitialized(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().Conjugate(world));
				}
			});
			
			FWorldDelegates::OnWorldBeginTearDown.AddLambda([](UWorld* world)
			{
#if WITH_EDITOR
				if (IZSharpRuntimeModule::Get().HasEngineAssembly())
#endif
				{
					FZRedFrameScope scope;
					FZWorld_Interop::GNotifyWorldTearingDown(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().Conjugate(world));
				}
			});
			
			FCoreUObjectDelegates::PreLoadMap.AddLambda([](const FString& map)
			{
#if WITH_EDITOR
				if (IZSharpRuntimeModule::Get().HasEngineAssembly())
#endif
				{
					FZRedFrameScope scope;
					FZWorld_Interop::GPreLoadMap(*map);
				}

			});
			
			FCoreUObjectDelegates::PostLoadMapWithWorld.AddLambda([](UWorld* world)
			{
#if WITH_EDITOR
				if (IZSharpRuntimeModule::Get().HasEngineAssembly())
#endif
				{
					FZRedFrameScope scope;
					FZWorld_Interop::GPostLoadMap(IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().Conjugate(world));
				}
			});
		}
	} GWorldDelegateListener;
}

ZSharp::FZConjugateHandle ZSharp::FZWorld_Interop::SpawnActor(FZConjugateHandle self, FZConjugateHandle cls, FZConjugateHandle transform, const TCHAR* name, FZConjugateHandle actorTemplate, FZConjugateHandle owner, FZConjugateHandle instigator, FZConjugateHandle overrideLevel, ESpawnActorCollisionHandlingMethod spawnCollisionHandlingOverride, ESpawnActorScaleMethod transformScaleMethod, FActorSpawnParameters::ESpawnActorNameMode nameMode, uint8 absolute, uint8 deferred)
{
	IZMasterAssemblyLoadContext* alc = IZSharpClr::Get().GetMasterAlc();
	FZConjugateRegistry_UObject& registry = alc->GetConjugateRegistry<FZConjugateRegistry_UObject>();

	UWorld* pSelf = registry.ConjugateUnsafe<UWorld>(self);
	UClass* pCls = registry.ConjugateUnsafe<UClass>(cls);
	FZSelfDescriptiveScriptStruct* sdTransform = alc->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>().ConjugateUnsafe(transform);
	const FTransform& spawnTransform = sdTransform->GetDescriptor() == TBaseStructure<FTransform>::Get() ? *sdTransform->GetTypedUnderlyingInstance<FTransform>() : FTransform::Identity;
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

EWorldType::Type ZSharp::FZWorld_Interop::GetWorldType(FZConjugateHandle self)
{
	auto pSelf = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe<UWorld>(self);
	return pSelf->WorldType;
}

ENetMode ZSharp::FZWorld_Interop::GetNetMode(FZConjugateHandle self)
{
	auto pSelf = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe<UWorld>(self);
	return pSelf->GetNetMode();
}


