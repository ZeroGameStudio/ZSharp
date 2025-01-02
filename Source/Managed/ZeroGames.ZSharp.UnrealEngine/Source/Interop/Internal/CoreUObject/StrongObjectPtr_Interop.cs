// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class StrongObjectPtr_Interop
{
	public static delegate* unmanaged<ConjugateHandle, IntPtr> Alloc;
	public static delegate* unmanaged<IntPtr, void> Free;
	
	public static delegate* unmanaged<IntPtr, IntPtr, uint8> Identical;
	public static delegate* unmanaged<IntPtr, int32> Hash;
	
	public static delegate* unmanaged<IntPtr, uint8, ConjugateHandle> Get;
	public static delegate* unmanaged<IntPtr, ConjugateHandle, void> Set;
	public static delegate* unmanaged<IntPtr, uint8, uint8> IsValid;
	public static delegate* unmanaged<IntPtr, uint8> IsNull;
}


