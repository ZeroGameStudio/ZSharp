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

void ZSharp::FZActor_Interop::GetActorTransform(FZConjugateHandle self, FTransform& transform)
{
	GUARD
	(
		auto pSelf = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe<AActor>(self);
		transform = pSelf->GetActorTransform();
	);
}

void ZSharp::FZActor_Interop::GetActorLocation(FZConjugateHandle self, FVector& location)
{
	GUARD
	(
		auto pSelf = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe<AActor>(self);
		location = pSelf->GetActorLocation();
	);
}

void ZSharp::FZActor_Interop::GetActorRotation(FZConjugateHandle self, FRotator& rotation)
{
	GUARD
	(
		auto pSelf = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe<AActor>(self);
		rotation = pSelf->GetActorRotation();
	);
}

void ZSharp::FZActor_Interop::GetActorScale(FZConjugateHandle self, FVector& scale)
{
	GUARD
	(
		auto pSelf = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe<AActor>(self);
		scale = pSelf->GetActorScale();
	);
}

uint8 ZSharp::FZActor_Interop::SetActorTransform(FZConjugateHandle self, const FTransform& newTransform, uint8 sweep, ETeleportType teleport)
{
	TRY
	{
		auto pSelf = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe<AActor>(self);
		return pSelf->SetActorTransform(newTransform, !!sweep, nullptr, teleport);
	}
	CATCHR(false)
}

uint8 ZSharp::FZActor_Interop::SetActorLocation(FZConjugateHandle self, const FVector& newLocation, uint8 sweep, ETeleportType teleport)
{
	TRY
	{
		auto pSelf = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe<AActor>(self);
		return pSelf->SetActorLocation(newLocation, !!sweep, nullptr, teleport);
	}
	CATCHR(false)
}

uint8 ZSharp::FZActor_Interop::SetActorRotation(FZConjugateHandle self, const FRotator& newRotator, ETeleportType teleport)
{
	TRY
	{
		auto pSelf = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe<AActor>(self);
		return pSelf->SetActorRotation(newRotator, teleport);
	}
	CATCHR(false)
}

uint8 ZSharp::FZActor_Interop::SetActorLocationAndRotation(FZConjugateHandle self, const FVector& newLocation, const FRotator& newRotator, uint8 sweep, ETeleportType teleport)
{
	TRY
	{
		auto pSelf = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe<AActor>(self);
		return pSelf->SetActorLocationAndRotation(newLocation, newRotator, !!sweep, nullptr, teleport);
	}
	CATCHR(false)
}

void ZSharp::FZActor_Interop::SetActorScale(FZConjugateHandle self, const FVector& newScale)
{
	GUARD
	(
		auto pSelf = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe<AActor>(self);
		return pSelf->SetActorScale3D(newScale);
	);
}

void ZSharp::FZActor_Interop::SetActorRelativeTransform(FZConjugateHandle self, const FTransform& newTransform, uint8 sweep, ETeleportType teleport)
{
	TRY
	{
		auto pSelf = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe<AActor>(self);
		return pSelf->SetActorRelativeTransform(newTransform, !!sweep, nullptr, teleport);
	}
	CATCHR(false)
}

void ZSharp::FZActor_Interop::SetActorRelativeLocation(FZConjugateHandle self, const FVector& newLocation, uint8 sweep, ETeleportType teleport)
{
	GUARD
	(
		auto pSelf = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe<AActor>(self);
		return pSelf->SetActorRelativeLocation(newLocation, !!sweep, nullptr, teleport);
	);
}

void ZSharp::FZActor_Interop::SetActorRelativeRotation(FZConjugateHandle self, const FRotator& newRotator, uint8 sweep, ETeleportType teleport)
{
	GUARD
	(
		auto pSelf = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe<AActor>(self);
		return pSelf->SetActorRelativeRotation(newRotator, !!sweep, nullptr, teleport);
	);
}

void ZSharp::FZActor_Interop::SetActorRelativeScale(FZConjugateHandle self, const FVector& newScale)
{
	GUARD
	(
		auto pSelf = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe<AActor>(self);
		return pSelf->SetActorRelativeScale3D(newScale);
	);
}

void ZSharp::FZActor_Interop::AddActorWorldTransform(FZConjugateHandle self, const FTransform& deltaTransform, uint8 sweep, ETeleportType teleport)
{
	GUARD
	(
		auto pSelf = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe<AActor>(self);
		return pSelf->AddActorWorldTransform(deltaTransform, !!sweep, nullptr, teleport);
	);
}

void ZSharp::FZActor_Interop::AddActorWorldTransformKeepScale(FZConjugateHandle self, const FTransform& deltaTransform, uint8 sweep, ETeleportType teleport)
{
	GUARD
	(
		auto pSelf = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe<AActor>(self);
		return pSelf->AddActorWorldTransformKeepScale(deltaTransform, !!sweep, nullptr, teleport);
	);
}

void ZSharp::FZActor_Interop::AddActorWorldOffset(FZConjugateHandle self, const FVector& deltaLocation, uint8 sweep, ETeleportType teleport)
{
	GUARD
	(
		auto pSelf = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe<AActor>(self);
		return pSelf->AddActorWorldOffset(deltaLocation, !!sweep, nullptr, teleport);
	);
}

void ZSharp::FZActor_Interop::AddActorWorldRotation(FZConjugateHandle self, const FRotator& deltaRotator, uint8 sweep, ETeleportType teleport)
{
	GUARD
	(
		auto pSelf = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe<AActor>(self);
		return pSelf->AddActorWorldRotation(deltaRotator, !!sweep, nullptr, teleport);
	);
}

void ZSharp::FZActor_Interop::AddActorLocalTransform(FZConjugateHandle self, const FTransform& deltaTransform, uint8 sweep, ETeleportType teleport)
{
	GUARD
	(
		auto pSelf = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe<AActor>(self);
		return pSelf->AddActorLocalTransform(deltaTransform, !!sweep, nullptr, teleport);
	);
}

void ZSharp::FZActor_Interop::AddActorLocalOffset(FZConjugateHandle self, const FVector& deltaLocation, uint8 sweep, ETeleportType teleport)
{
	GUARD
	(
		auto pSelf = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe<AActor>(self);
		return pSelf->AddActorLocalOffset(deltaLocation, !!sweep, nullptr, teleport);
	);
}

void ZSharp::FZActor_Interop::AddActorLocalRotation(FZConjugateHandle self, const FRotator& deltaRotator, uint8 sweep, ETeleportType teleport)
{
	GUARD
	(
		auto pSelf = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe<AActor>(self);
		return pSelf->AddActorLocalRotation(deltaRotator, !!sweep, nullptr, teleport);
	);
}


