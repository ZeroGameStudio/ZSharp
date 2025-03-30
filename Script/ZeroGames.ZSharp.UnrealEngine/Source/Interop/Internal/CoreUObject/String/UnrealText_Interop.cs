// Copyright Zero Games. All Rights Reserved.

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class UnrealText_Interop
{
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, char*> GetData;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, char*, void> SetData;
}


