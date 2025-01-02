// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class UnrealClass_Interop
{
	public static delegate* unmanaged<ConjugateHandle, uint8, ConjugateHandle> GetDefaultObject;
	public static delegate* unmanaged<ConjugateHandle, uint8> IsInterface;
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, uint8> ImplementsInterface;
}


