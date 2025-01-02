// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class UnrealString_Interop
{
	public static delegate* unmanaged<ConjugateHandle, char*> GetData;
	public static delegate* unmanaged<ConjugateHandle, char*, void> SetData;
}


