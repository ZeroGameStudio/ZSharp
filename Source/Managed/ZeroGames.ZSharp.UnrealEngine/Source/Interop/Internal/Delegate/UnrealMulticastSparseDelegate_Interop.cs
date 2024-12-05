// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class UnrealMulticastSparseDelegate_Interop
{
	public static delegate* unmanaged<ref ZCallBuffer, EZCallErrorCode> Broadcast;
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, char*, void> AddUnrealFunction;
	public static delegate* unmanaged<ConjugateHandle, GCHandle, ConjugateHandle> AddManagedDelegate;
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, char*, void> Remove;
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, void> RemoveAll;
	public static delegate* unmanaged<ConjugateHandle, void> Clear;
	public static delegate* unmanaged<ConjugateHandle, uint8> IsBound;
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, uint8> IsBoundToObject;
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, char*, uint8> Contains;
}


