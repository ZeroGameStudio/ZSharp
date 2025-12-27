// Copyright Zero Games. All Rights Reserved.

using ZeroGames.Extensions.Math;

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class Actor_Interop
{
	public static delegate* unmanaged<ConjugateHandle, in Transform, void> FinishSpawning;
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, uint8, ConjugateHandle> AddComponent;
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, void> FinishAddComponent;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, ENetMode> GetNetMode;

	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, out Transform, void> GetActorTransform;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, out Vector, void> GetActorLocation;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, out Rotator, void> GetActorRotation;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, out Vector, void> GetActorScale;
	
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, in Transform, uint8, uint8, uint8> SetActorTransform;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, in Vector, uint8, uint8, uint8> SetActorLocation;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, in Rotator, uint8, uint8> SetActorRotation;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, in Vector, in Rotator, uint8, uint8, uint8> SetActorLocationAndRotation;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, in Vector, void> SetActorScale;
	
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, in Transform, uint8, uint8, void> SetActorRelativeTransform;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, in Vector, uint8, uint8, void> SetActorRelativeLocation;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, in Rotator, uint8, uint8, void> SetActorRelativeRotation;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, in Vector, void> SetActorRelativeScale;
	
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, in Transform, uint8, uint8, void> AddActorWorldTransform;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, in Transform, uint8, uint8, void> AddActorWorldTransformKeepScale;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, in Vector, uint8, uint8, void> AddActorWorldOffset;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, in Rotator, uint8, uint8, void> AddActorWorldRotation;
	
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, in Transform, uint8, uint8, void> AddActorLocalTransform;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, in Vector, uint8, uint8, void> AddActorLocalOffset;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, in Rotator, uint8, uint8, void> AddActorLocalRotation;
}


