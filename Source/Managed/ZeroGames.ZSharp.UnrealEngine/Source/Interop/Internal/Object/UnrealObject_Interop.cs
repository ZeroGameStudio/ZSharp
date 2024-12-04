// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class UnrealObject_Interop
{
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle> GetClass;
	public static delegate* unmanaged<ConjugateHandle, IntPtr, void> GetName;
	public static delegate* unmanaged<ConjugateHandle, IntPtr, void> GetPathName;
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle> GetOuter;
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, uint8> IsA;
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, uint8> Implements;
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, uint8> IsIn;
	public static delegate* unmanaged<ConjugateHandle, char*, ConjugateHandle, uint8> Rename;
}


