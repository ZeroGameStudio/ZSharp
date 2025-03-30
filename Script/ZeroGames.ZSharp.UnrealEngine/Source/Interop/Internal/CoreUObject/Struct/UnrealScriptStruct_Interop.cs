// Copyright Zero Games. All Rights Reserved.

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class UnrealScriptStruct_Interop
{
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, void> Copy;
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, uint8> Identical;
}


