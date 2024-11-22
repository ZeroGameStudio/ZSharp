// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class SubclassOf_Interop
{
	
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, void> Copy;
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, uint8> Identical;
	public static delegate* unmanaged<ConjugateHandle, int32> Hash;
	
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle> Get;
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, void> Set;
	public static delegate* unmanaged<ConjugateHandle, uint8> IsValid;
	
}


