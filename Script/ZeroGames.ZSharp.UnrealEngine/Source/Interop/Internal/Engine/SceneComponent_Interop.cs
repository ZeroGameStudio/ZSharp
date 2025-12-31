// Copyright Zero Games. All Rights Reserved.

using ZeroGames.Extensions.Math;

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class SceneComponent_Interop
{
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, out Transform, void> GetComponentTransform;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, out Vector, void> GetComponentLocation;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, out Rotator, void> GetComponentRotation;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, out Vector, void> GetComponentScale;
	
	// Socket APIs are virtual thus we don't know what they do, don't use [SuppressGCTransition].
	public static delegate* unmanaged<ConjugateHandle, char*, int32, out Transform, void> GetSocketTransform;
	public static delegate* unmanaged<ConjugateHandle, char*, int32, out Vector, void> GetSocketLocation;
	public static delegate* unmanaged<ConjugateHandle, char*, int32, out Rotator, void> GetSocketRotation;
	public static delegate* unmanaged<ConjugateHandle, char*, int32, out Vector, void> GetSocketScale;
	
	public static delegate* unmanaged<ConjugateHandle, in Transform, uint8, uint8, void> SetWorldTransform;
	public static delegate* unmanaged<ConjugateHandle, in Vector, uint8, uint8, void> SetWorldLocation;
	public static delegate* unmanaged<ConjugateHandle, in Rotator, uint8, uint8, void> SetWorldRotation;
	public static delegate* unmanaged<ConjugateHandle, in Vector, in Rotator, uint8, uint8, void> SetWorldLocationAndRotation;
	public static delegate* unmanaged<ConjugateHandle, in Vector, void> SetWorldScale;
	
	public static delegate* unmanaged<ConjugateHandle, in Transform, uint8, uint8, void> SetRelativeTransform;
	public static delegate* unmanaged<ConjugateHandle, in Vector, uint8, uint8, void> SetRelativeLocation;
	public static delegate* unmanaged<ConjugateHandle, in Rotator, uint8, uint8, void> SetRelativeRotation;
	public static delegate* unmanaged<ConjugateHandle, in Vector, void> SetRelativeScale;
	
	public static delegate* unmanaged<ConjugateHandle, in Transform, uint8, uint8, void> AddWorldTransform;
	public static delegate* unmanaged<ConjugateHandle, in Transform, uint8, uint8, void> AddWorldTransformKeepScale;
	public static delegate* unmanaged<ConjugateHandle, in Vector, uint8, uint8, void> AddWorldOffset;
	public static delegate* unmanaged<ConjugateHandle, in Rotator, uint8, uint8, void> AddWorldRotation;
	
	public static delegate* unmanaged<ConjugateHandle, in Transform, uint8, uint8, void> AddLocalTransform;
	public static delegate* unmanaged<ConjugateHandle, in Vector, uint8, uint8, void> AddLocalOffset;
	public static delegate* unmanaged<ConjugateHandle, in Rotator, uint8, uint8, void> AddLocalRotation;
}


