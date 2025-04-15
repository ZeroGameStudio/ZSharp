// Copyright Zero Games. All Rights Reserved.

#include "ZActor_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"

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

void ZSharp::FZActor_Interop::FinishSpawning(FZConjugateHandle self)
{
	auto pSelf = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe<AActor>(self);
	pSelf->FinishSpawning(FTransform::Identity, true);
}

ZSharp::FZConjugateHandle ZSharp::FZActor_Interop::AddComponent(FZConjugateHandle self, FZConjugateHandle componentClass, uint8 defer)
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

void ZSharp::FZActor_Interop::FinishAddComponent(FZConjugateHandle self, FZConjugateHandle component)
{
	auto& registry = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>();
	auto pSelf = registry.ConjugateUnsafe<AActor>(self);
	auto pComponent = registry.ConjugateUnsafe<UActorComponent>(component);

	ZActor_Interop_Private::FinishAddComponent(pSelf, pComponent);
}

ENetMode ZSharp::FZActor_Interop::GetNetMode(FZConjugateHandle self)
{
	auto pSelf = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe<AActor>(self);
	return pSelf->GetNetMode();
}


