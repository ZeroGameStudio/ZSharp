// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine;

public static unsafe class UnrealMulticastInlineDelegate_Interop
{
	public static delegate* unmanaged<ZCallBuffer*, EZCallErrorCode> Broadcast;
	public static delegate* unmanaged<ConjugateHandle, GCHandle, ConjugateHandle> AddManagedDelegate;
	internal static delegate* unmanaged<ConjugateHandle, ConjugateHandle, char*, void> AddUnrealFunction;
	internal static delegate* unmanaged<ConjugateHandle, ConjugateHandle, char*, void> Remove;
	internal static delegate* unmanaged<ConjugateHandle, ConjugateHandle, void> RemoveAll;
	internal static delegate* unmanaged<ConjugateHandle, void> Clear;
	internal static delegate* unmanaged<ConjugateHandle, uint8> IsBound;
	internal static delegate* unmanaged<ConjugateHandle, ConjugateHandle, uint8> IsBoundToObject;
	internal static delegate* unmanaged<ConjugateHandle, ConjugateHandle, char*, uint8> Contains;
}


