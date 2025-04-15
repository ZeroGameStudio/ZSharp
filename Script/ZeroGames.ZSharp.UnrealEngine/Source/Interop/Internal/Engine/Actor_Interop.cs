// Copyright Zero Games. All Rights Reserved.

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class Actor_Interop
{
	public static delegate* unmanaged<ConjugateHandle, void> FinishSpawning;
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, uint8, ConjugateHandle> AddComponent;
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, void> FinishAddComponent;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, ENetMode> GetNetMode;
}


