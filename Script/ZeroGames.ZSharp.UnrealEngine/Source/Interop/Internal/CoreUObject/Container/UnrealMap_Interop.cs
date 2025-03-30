// Copyright Zero Games. All Rights Reserved.

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class UnrealMap_Interop
{
	public static delegate* unmanaged<ConjugateHandle, ZCallBufferSlot, ZCallBufferSlot, uint8> TryAdd;
	public static delegate* unmanaged<ConjugateHandle, ZCallBufferSlot, uint8> Remove;
	public static delegate* unmanaged<ConjugateHandle, void> Clear;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, ZCallBufferSlot, uint8> Contains;
	public static delegate* unmanaged<ConjugateHandle, ZCallBufferSlot, ref ZCallBufferSlot, uint8> Get;
	public static delegate* unmanaged<ConjugateHandle, ZCallBufferSlot, ZCallBufferSlot, void> Set;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, int32> Count;
	
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, IntPtr> CreateEnumerator;
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, void> ReleaseEnumerator;
	public static delegate* unmanaged[SuppressGCTransition]<IntPtr, uint8> EnumeratorMoveNext;
	public static delegate* unmanaged<IntPtr, ConjugateHandle, ref ZCallBufferSlot, ref ZCallBufferSlot, void> EnumeratorCurrent;
}


