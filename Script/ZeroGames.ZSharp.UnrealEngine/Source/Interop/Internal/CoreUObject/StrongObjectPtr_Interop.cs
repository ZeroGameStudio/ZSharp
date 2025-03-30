// Copyright Zero Games. All Rights Reserved.

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class StrongObjectPtr_Interop
{
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, IntPtr> Alloc;
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, void> Free;
	
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, IntPtr, uint8> Identical;
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, int32> Hash;
	
	public static delegate* unmanaged<IntPtr, uint8, ConjugateHandle> Get;
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, ConjugateHandle, void> Set;
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, uint8, uint8> IsValid;
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, uint8> IsNull;
}


