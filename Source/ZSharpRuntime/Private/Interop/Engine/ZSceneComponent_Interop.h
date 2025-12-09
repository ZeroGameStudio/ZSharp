// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Conjugate/ZConjugateHandle.h"

namespace ZSharp
{
	struct FZSceneComponent_Interop
	{
		static void GetComponentTransform(FZConjugateHandle self, FTransform& transform);
		static void GetComponentLocation(FZConjugateHandle self, FVector& location);
		static void GetComponentRotation(FZConjugateHandle self, FRotator& rotation);
		static void GetComponentScale(FZConjugateHandle self, FVector& scale);
		
		static void GetSocketTransform(FZConjugateHandle self, const TCHAR* socketName, ERelativeTransformSpace transformSpace, FTransform& transform);
		static void GetSocketLocation(FZConjugateHandle self, const TCHAR* socketName, ERelativeTransformSpace transformSpace, FVector& location);
		static void GetSocketRotation(FZConjugateHandle self, const TCHAR* socketName, ERelativeTransformSpace transformSpace, FRotator& rotation);
		static void GetSocketScale(FZConjugateHandle self, const TCHAR* socketName, ERelativeTransformSpace transformSpace, FVector& scale);
		
		static void SetWorldTransform(FZConjugateHandle self, const FTransform& newTransform, uint8 sweep, ETeleportType teleport);
		static void SetWorldLocation(FZConjugateHandle self, const FVector& newLocation, uint8 sweep, ETeleportType teleport);
		static void SetWorldRotation(FZConjugateHandle self, const FRotator& newRotator, uint8 sweep, ETeleportType teleport);
		static void SetWorldLocationAndRotation(FZConjugateHandle self, const FVector& newLocation, const FRotator& newRotator, uint8 sweep, ETeleportType teleport);
		static void SetWorldScale(FZConjugateHandle self, const FVector& newScale);
		
		static void SetRelativeTransform(FZConjugateHandle self, const FTransform& newTransform, uint8 sweep, ETeleportType teleport);
		static void SetRelativeLocation(FZConjugateHandle self, const FVector& newLocation, uint8 sweep, ETeleportType teleport);
		static void SetRelativeRotation(FZConjugateHandle self, const FRotator& newRotator, uint8 sweep, ETeleportType teleport);
		static void SetRelativeScale(FZConjugateHandle self, const FVector& newScale);
		
		static void AddWorldTransform(FZConjugateHandle self, const FTransform& deltaTransform, uint8 sweep, ETeleportType teleport);
		static void AddWorldTransformKeepScale(FZConjugateHandle self, const FTransform& deltaTransform, uint8 sweep, ETeleportType teleport);
		static void AddWorldOffset(FZConjugateHandle self, const FVector& deltaLocation, uint8 sweep, ETeleportType teleport);
		static void AddWorldRotation(FZConjugateHandle self, const FRotator& deltaRotator, uint8 sweep, ETeleportType teleport);
		
		static void AddLocalTransform(FZConjugateHandle self, const FTransform& deltaTransform, uint8 sweep, ETeleportType teleport);
		static void AddLocalOffset(FZConjugateHandle self, const FVector& deltaLocation, uint8 sweep, ETeleportType teleport);
		static void AddLocalRotation(FZConjugateHandle self, const FRotator& deltaRotator, uint8 sweep, ETeleportType teleport);
	};
}


