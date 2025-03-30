// Copyright Zero Games. All Rights Reserved.

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class UnrealName_Interop
{
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, IntPtr, void> GetData;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, char*, void> SetData;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, uint8> IsNone;
}


