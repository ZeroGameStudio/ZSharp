// Copyright Zero Games. All Rights Reserved.

#include "ZActor_Interop.h"

#include "Conjugate/ZConjugateUnsafe.h"
#include "Interop/ZInteropExceptionHelper.h"
#include "Interop/ZInteropHelper.h"

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

void ZSharp::FZActor_Interop::FinishSpawning(FZConjugateHandle self, const FTransform& transform)
{
	GUARD
	(
		auto pSelf = ConjugateUnsafe<AActor>(self);
		FTransform spawnTransform;
		FZInteropHelper::CopyTransformUnaligned(transform, spawnTransform);
		pSelf->FinishSpawning(spawnTransform);
	);
}

ZSharp::FZConjugateHandle ZSharp::FZActor_Interop::AddComponent(FZConjugateHandle self, FZConjugateHandle componentClass, uint8 defer)
{
	TRY
	{
		auto pSelf = ConjugateUnsafe<AActor>(self);
		auto pComponentClass = ConjugateUnsafe<UClass>(componentClass);
		if (!pComponentClass)
		{
			return {};
		}
	
		UActorComponent* component = NewObject<UActorComponent>(pSelf, pComponentClass);
		ZActor_Interop_Private::AttachComponent(pSelf, component);

		if (!defer)
		{
			ZActor_Interop_Private::FinishAddComponent(pSelf, component);
		}
	
		return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().Conjugate(component);
	}
	CATCHR({})
}

void ZSharp::FZActor_Interop::FinishAddComponent(FZConjugateHandle self, FZConjugateHandle component)
{
	GUARD
	(
		auto pSelf = ConjugateUnsafe<AActor>(self);
		auto pComponent = ConjugateUnsafe<UActorComponent>(component);
		if (!pComponent)
		{
			return;
		}
		
		ZActor_Interop_Private::FinishAddComponent(pSelf, pComponent);
	);
}

ENetMode ZSharp::FZActor_Interop::GetNetMode(FZConjugateHandle self)
{
	TRY
	{
		return ConjugateUnsafe<AActor>(self)->GetNetMode();
	}
	CATCHR(NM_Standalone)
}

void ZSharp::FZActor_Interop::GetActorTransform(FZConjugateHandle self, FTransform& transform)
{
	GUARD(FZInteropHelper::CopyTransformUnaligned(ConjugateUnsafe<AActor>(self)->GetActorTransform(), transform));
}

void ZSharp::FZActor_Interop::GetActorLocation(FZConjugateHandle self, FVector& location)
{
	GUARD(location = ConjugateUnsafe<AActor>(self)->GetActorLocation());
}

void ZSharp::FZActor_Interop::GetActorRotation(FZConjugateHandle self, FRotator& rotation)
{
	GUARD(rotation = ConjugateUnsafe<AActor>(self)->GetActorRotation());
}

void ZSharp::FZActor_Interop::GetActorScale(FZConjugateHandle self, FVector& scale)
{
	GUARD(scale = ConjugateUnsafe<AActor>(self)->GetActorScale());
}

uint8 ZSharp::FZActor_Interop::SetActorTransform(FZConjugateHandle self, const FTransform& newTransform, uint8 sweep, ETeleportType teleport)
{
	TRY
	{
		FTransform trans;
		FZInteropHelper::CopyTransformUnaligned(newTransform, trans);
		return ConjugateUnsafe<AActor>(self)->SetActorTransform(trans, !!sweep, nullptr, teleport);
	}
	CATCHR(false)
}

uint8 ZSharp::FZActor_Interop::SetActorLocation(FZConjugateHandle self, const FVector& newLocation, uint8 sweep, ETeleportType teleport)
{
	TRY
	{
		return ConjugateUnsafe<AActor>(self)->SetActorLocation(newLocation, !!sweep, nullptr, teleport);
	}
	CATCHR(false)
}

uint8 ZSharp::FZActor_Interop::SetActorRotation(FZConjugateHandle self, const FRotator& newRotator, ETeleportType teleport)
{
	TRY
	{
		return ConjugateUnsafe<AActor>(self)->SetActorRotation(newRotator, teleport);
	}
	CATCHR(false)
}

uint8 ZSharp::FZActor_Interop::SetActorLocationAndRotation(FZConjugateHandle self, const FVector& newLocation, const FRotator& newRotator, uint8 sweep, ETeleportType teleport)
{
	TRY
	{
		return ConjugateUnsafe<AActor>(self)->SetActorLocationAndRotation(newLocation, newRotator, !!sweep, nullptr, teleport);
	}
	CATCHR(false)
}

void ZSharp::FZActor_Interop::SetActorScale(FZConjugateHandle self, const FVector& newScale)
{
	GUARD
	(
		return ConjugateUnsafe<AActor>(self)->SetActorScale3D(newScale);
	);
}

void ZSharp::FZActor_Interop::SetActorRelativeTransform(FZConjugateHandle self, const FTransform& newTransform, uint8 sweep, ETeleportType teleport)
{
	GUARD
	(
		FTransform trans;
		FZInteropHelper::CopyTransformUnaligned(newTransform, trans);
		return ConjugateUnsafe<AActor>(self)->SetActorRelativeTransform(trans, !!sweep, nullptr, teleport)
	);
}

void ZSharp::FZActor_Interop::SetActorRelativeLocation(FZConjugateHandle self, const FVector& newLocation, uint8 sweep, ETeleportType teleport)
{
	GUARD
	(
		return ConjugateUnsafe<AActor>(self)->SetActorRelativeLocation(newLocation, !!sweep, nullptr, teleport);
	);
}

void ZSharp::FZActor_Interop::SetActorRelativeRotation(FZConjugateHandle self, const FRotator& newRotator, uint8 sweep, ETeleportType teleport)
{
	GUARD
	(
		return ConjugateUnsafe<AActor>(self)->SetActorRelativeRotation(newRotator, !!sweep, nullptr, teleport);
	);
}

void ZSharp::FZActor_Interop::SetActorRelativeScale(FZConjugateHandle self, const FVector& newScale)
{
	GUARD
	(
		return ConjugateUnsafe<AActor>(self)->SetActorRelativeScale3D(newScale);
	);
}

void ZSharp::FZActor_Interop::AddActorWorldTransform(FZConjugateHandle self, const FTransform& deltaTransform, uint8 sweep, ETeleportType teleport)
{
	GUARD
	(
		FTransform trans;
		FZInteropHelper::CopyTransformUnaligned(deltaTransform, trans);
		return ConjugateUnsafe<AActor>(self)->AddActorWorldTransform(trans, !!sweep, nullptr, teleport);
	);
}

void ZSharp::FZActor_Interop::AddActorWorldTransformKeepScale(FZConjugateHandle self, const FTransform& deltaTransform, uint8 sweep, ETeleportType teleport)
{
	GUARD
	(
		FTransform trans;
		FZInteropHelper::CopyTransformUnaligned(deltaTransform, trans);
		return ConjugateUnsafe<AActor>(self)->AddActorWorldTransformKeepScale(trans, !!sweep, nullptr, teleport);
	);
}

void ZSharp::FZActor_Interop::AddActorWorldOffset(FZConjugateHandle self, const FVector& deltaLocation, uint8 sweep, ETeleportType teleport)
{
	GUARD
	(
		return ConjugateUnsafe<AActor>(self)->AddActorWorldOffset(deltaLocation, !!sweep, nullptr, teleport);
	);
}

void ZSharp::FZActor_Interop::AddActorWorldRotation(FZConjugateHandle self, const FRotator& deltaRotator, uint8 sweep, ETeleportType teleport)
{
	GUARD
	(
		return ConjugateUnsafe<AActor>(self)->AddActorWorldRotation(deltaRotator, !!sweep, nullptr, teleport);
	);
}

void ZSharp::FZActor_Interop::AddActorLocalTransform(FZConjugateHandle self, const FTransform& deltaTransform, uint8 sweep, ETeleportType teleport)
{
	GUARD
	(
		FTransform trans;
		FZInteropHelper::CopyTransformUnaligned(deltaTransform, trans);
		return ConjugateUnsafe<AActor>(self)->AddActorLocalTransform(trans, !!sweep, nullptr, teleport);
	);
}

void ZSharp::FZActor_Interop::AddActorLocalOffset(FZConjugateHandle self, const FVector& deltaLocation, uint8 sweep, ETeleportType teleport)
{
	GUARD
	(
		return ConjugateUnsafe<AActor>(self)->AddActorLocalOffset(deltaLocation, !!sweep, nullptr, teleport);
	);
}

void ZSharp::FZActor_Interop::AddActorLocalRotation(FZConjugateHandle self, const FRotator& deltaRotator, uint8 sweep, ETeleportType teleport)
{
	GUARD
	(
		return ConjugateUnsafe<AActor>(self)->AddActorLocalRotation(deltaRotator, !!sweep, nullptr, teleport);
	);
}


