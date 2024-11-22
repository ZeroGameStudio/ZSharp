// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class UnrealName_Interop
{
	public static delegate* unmanaged<ConjugateHandle, IntPtr, void> GetData;
	public static delegate* unmanaged<ConjugateHandle, char*, void> SetData;
	public static delegate* unmanaged<ConjugateHandle, uint8> IsNone;
}


