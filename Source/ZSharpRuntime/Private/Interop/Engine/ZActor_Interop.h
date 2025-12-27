// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Conjugate/ZConjugateHandle.h"

namespace ZSharp
{
	struct FZActor_Interop
	{
		static void FinishSpawning(FZConjugateHandle self, const FTransform& transform);
		static FZConjugateHandle AddComponent(FZConjugateHandle self, FZConjugateHandle componentClass, uint8 defer);
		static void FinishAddComponent(FZConjugateHandle self, FZConjugateHandle component);
		static ENetMode GetNetMode(FZConjugateHandle self);
		
		static void GetActorTransform(FZConjugateHandle self, FTransform& transform);
		static void GetActorLocation(FZConjugateHandle self, FVector& location);
		static void GetActorRotation(FZConjugateHandle self, FRotator& rotation);
		static void GetActorScale(FZConjugateHandle self, FVector& scale);
		
		static uint8 SetActorTransform(FZConjugateHandle self, const FTransform& newTransform, uint8 sweep, ETeleportType teleport);
		static uint8 SetActorLocation(FZConjugateHandle self, const FVector& newLocation, uint8 sweep, ETeleportType teleport);
		static uint8 SetActorRotation(FZConjugateHandle self, const FRotator& newRotator, ETeleportType teleport);
		static uint8 SetActorLocationAndRotation(FZConjugateHandle self, const FVector& newLocation, const FRotator& newRotator, uint8 sweep, ETeleportType teleport);
		static void SetActorScale(FZConjugateHandle self, const FVector& newScale);
		
		static void SetActorRelativeTransform(FZConjugateHandle self, const FTransform& newTransform, uint8 sweep, ETeleportType teleport);
		static void SetActorRelativeLocation(FZConjugateHandle self, const FVector& newLocation, uint8 sweep, ETeleportType teleport);
		static void SetActorRelativeRotation(FZConjugateHandle self, const FRotator& newRotator, uint8 sweep, ETeleportType teleport);
		static void SetActorRelativeScale(FZConjugateHandle self, const FVector& newScale);
		
		static void AddActorWorldTransform(FZConjugateHandle self, const FTransform& deltaTransform, uint8 sweep, ETeleportType teleport);
		static void AddActorWorldTransformKeepScale(FZConjugateHandle self, const FTransform& deltaTransform, uint8 sweep, ETeleportType teleport);
		static void AddActorWorldOffset(FZConjugateHandle self, const FVector& deltaLocation, uint8 sweep, ETeleportType teleport);
		static void AddActorWorldRotation(FZConjugateHandle self, const FRotator& deltaRotator, uint8 sweep, ETeleportType teleport);
		
		static void AddActorLocalTransform(FZConjugateHandle self, const FTransform& deltaTransform, uint8 sweep, ETeleportType teleport);
		static void AddActorLocalOffset(FZConjugateHandle self, const FVector& deltaLocation, uint8 sweep, ETeleportType teleport);
		static void AddActorLocalRotation(FZConjugateHandle self, const FRotator& deltaRotator, uint8 sweep, ETeleportType teleport);
	};
}


