// Copyright Zero Games. All Rights Reserved.

#include "ZActor_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"
#include "Conjugate/ZStrangeConjugateRegistries.h"
#include "Interop/ZInteropExceptionHelper.h"

namespace ZSharp::ZActor_Interop_Private
{
	static void AttachComponent(AActor* actor, UActorComponent* component)
	{
		if (auto sceneComponent = Cast<USceneComponent>(component))
		{
			if (actor->GetRootComponent())
			{
				sceneComponent->SetupAttachment(actor->GetRootComponent());
			}
			else
			{
				actor->SetRootComponent(sceneComponent);
			}
		}
	}
	
	static void FinishAddComponent(AActor* actor, UActorComponent* component)
	{
		component->RegisterComponent();
	}
}

void ZSharp::FZActor_Interop::FinishSpawning(FZConjugateHandle self, FZConjugateHandle transform)
{
	GUARD
	(
		IZMasterAssemblyLoadContext* alc = IZSharpClr::Get().GetMasterAlc();
		auto pSelf = alc->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe<AActor>(self);
		FZSelfDescriptiveScriptStruct* sdTransform = alc->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>().ConjugateUnsafe(transform);
		const FTransform& spawnTransform = sdTransform && sdTransform->GetDescriptor() == TBaseStructure<FTransform>::Get() ? *sdTransform->GetTypedUnderlyingInstance<FTransform>() : FTransform::Identity;
		pSelf->FinishSpawning(spawnTransform);
	);
}

ZSharp::FZConjugateHandle ZSharp::FZActor_Interop::AddComponent(FZConjugateHandle self, FZConjugateHandle componentClass, uint8 defer)
{
	TRY
	{
		auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
		auto pSelf = registry.ConjugateUnsafe<AActor>(self);
		auto pComponentClass = registry.ConjugateUnsafe<UClass>(componentClass);
	
		UActorComponent* component = NewObject<UActorComponent>(pSelf, pComponentClass);
		ZActor_Interop_Private::AttachComponent(pSelf, component);

		if (!defer)
		{
			ZActor_Interop_Private::FinishAddComponent(pSelf, component);
		}
	
		return registry.Conjugate(component);
	}
	CATCHR({})
}

void ZSharp::FZActor_Interop::FinishAddComponent(FZConjugateHandle self, FZConjugateHandle component)
{
	GUARD
	(
		auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
		auto pSelf = registry.ConjugateUnsafe<AActor>(self);
		auto pComponent = registry.ConjugateUnsafe<UActorComponent>(component);
		
		ZActor_Interop_Private::FinishAddComponent(pSelf, pComponent);
	);
}

ENetMode ZSharp::FZActor_Interop::GetNetMode(FZConjugateHandle self)
{
	TRY
	{
		auto pSelf = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe<AActor>(self);
		return pSelf->GetNetMode();
	}
	CATCHR(NM_Standalone)
}


