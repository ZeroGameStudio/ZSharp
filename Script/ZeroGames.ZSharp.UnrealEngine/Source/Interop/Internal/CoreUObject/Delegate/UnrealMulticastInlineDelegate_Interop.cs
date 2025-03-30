// Copyright Zero Games. All Rights Reserved.

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class UnrealMulticastInlineDelegate_Interop
{
	public static delegate* unmanaged<ZCallBuffer*, EZCallErrorCode> Broadcast;
	public static delegate* unmanaged<ConjugateHandle, GCHandle, ConjugateHandle> AddStatelessManagedDelegate;
	public static delegate* unmanaged<ConjugateHandle, GCHandle, GCHandle, ConjugateHandle> AddStatefulManagedDelegate;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, ConjugateHandle, char*, void> AddUnrealFunction;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, ConjugateHandle, char*, void> Remove;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, ConjugateHandle, void> RemoveAll;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, void> Clear;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, uint8> IsBound;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, ConjugateHandle, uint8> IsBoundToObject;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, ConjugateHandle, char*, uint8> Contains;
}


