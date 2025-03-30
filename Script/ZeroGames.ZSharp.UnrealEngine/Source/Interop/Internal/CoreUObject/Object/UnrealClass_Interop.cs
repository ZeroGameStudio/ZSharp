// Copyright Zero Games. All Rights Reserved.

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class UnrealClass_Interop
{
	public static delegate* unmanaged<ConjugateHandle, uint8, ConjugateHandle> GetDefaultObject;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, uint8> IsInterface;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, ConjugateHandle, uint8> ImplementsInterface;
}


