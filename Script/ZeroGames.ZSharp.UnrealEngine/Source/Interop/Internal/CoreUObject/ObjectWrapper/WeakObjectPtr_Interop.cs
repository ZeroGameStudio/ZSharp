// Copyright Zero Games. All Rights Reserved.

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class WeakObjectPtr_Interop
{
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, ConjugateHandle, void> Copy;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, ConjugateHandle, uint8> Identical;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, int32> Hash;
	
	public static delegate* unmanaged<ConjugateHandle, uint8, ConjugateHandle> Get;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, ConjugateHandle, void> Set;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, uint8, uint8> IsValid;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, uint8> IsNull;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, uint8, uint8> IsStale;
}


