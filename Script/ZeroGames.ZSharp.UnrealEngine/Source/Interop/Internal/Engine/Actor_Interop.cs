// Copyright Zero Games. All Rights Reserved.

using ZeroGames.Extensions.Math;

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class Actor_Interop
{
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, void> FinishSpawning;
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, uint8, ConjugateHandle> AddComponent;
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, void> FinishAddComponent;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, ENetMode> GetNetMode;

	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, out Transform, void> GetActorTransform;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, out Vector, void> GetActorLocation;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, out Rotator, void> GetActorRotation;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, out Vector, void> GetActorScale;
	
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, in Transform, uint8, ETeleportType, uint8> SetActorTransform;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, in Vector, uint8, ETeleportType, uint8> SetActorLocation;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, in Rotator, ETeleportType, uint8> SetActorRotation;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, in Vector, in Rotator, uint8, ETeleportType, uint8> SetActorLocationAndRotation;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, in Vector, void> SetActorScale;
	
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, in Transform, uint8, ETeleportType, void> SetActorRelativeTransform;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, in Vector, uint8, ETeleportType, void> SetActorRelativeLocation;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, in Rotator, uint8, ETeleportType, void> SetActorRelativeRotation;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, in Vector, void> SetActorRelativeScale;
	
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, in Transform, uint8, ETeleportType, void> AddActorWorldTransform;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, in Transform, uint8, ETeleportType, void> AddActorWorldTransformKeepScale;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, in Vector, uint8, ETeleportType, void> AddActorWorldOffset;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, in Rotator, uint8, ETeleportType, void> AddActorWorldRotation;
	
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, in Transform, uint8, ETeleportType, void> AddActorLocalTransform;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, in Vector, uint8, ETeleportType, void> AddActorLocalOffset;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, in Rotator, uint8, ETeleportType, void> AddActorLocalRotation;
}


