// Copyright Zero Games. All Rights Reserved.

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class UnrealDelegate_Interop
{
	public static delegate* unmanaged<ZCallBuffer*, EZCallErrorCode> Execute;
	public static delegate* unmanaged<ConjugateHandle, GCHandle, ConjugateHandle> BindStatelessManagedDelegate;
	public static delegate* unmanaged<ConjugateHandle, GCHandle, GCHandle, ConjugateHandle> BindStatefulManagedDelegate;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, ConjugateHandle, char*, void> BindUnrealFunction;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, void> Unbind;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, uint8> IsBound;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, ConjugateHandle, uint8> IsBoundToObject;
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle> GetObject;
	public static delegate* unmanaged<ConjugateHandle, IntPtr, void> GetFunctionName;
}


