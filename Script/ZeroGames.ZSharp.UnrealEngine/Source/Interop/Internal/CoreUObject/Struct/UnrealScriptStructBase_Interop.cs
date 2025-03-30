// Copyright Zero Games. All Rights Reserved.

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class UnrealScriptStructBase_Interop
{
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, void> Copy;
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, uint8> Identical;
	
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle> GetScriptStruct;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, ConjugateHandle, uint8> IsA;
}


