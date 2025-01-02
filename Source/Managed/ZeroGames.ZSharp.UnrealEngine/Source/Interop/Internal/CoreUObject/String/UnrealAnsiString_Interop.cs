// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class UnrealAnsiString_Interop
{
	public static delegate* unmanaged<ConjugateHandle, IntPtr, void> GetData;
	public static delegate* unmanaged<ConjugateHandle, char*, void> SetData;
}


