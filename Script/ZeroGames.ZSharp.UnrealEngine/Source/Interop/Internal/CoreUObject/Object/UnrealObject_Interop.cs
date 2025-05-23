﻿// Copyright Zero Games. All Rights Reserved.

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class UnrealObject_Interop
{
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle> GetClass;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, IntPtr, void> GetName;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, IntPtr, void> GetPathName;
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle> GetOuter;
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle> GetWorld;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, ConjugateHandle, uint8> IsA;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, ConjugateHandle, uint8> Implements;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, ConjugateHandle, uint8> IsIn;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, char*, ConjugateHandle, uint8> Rename;
}


