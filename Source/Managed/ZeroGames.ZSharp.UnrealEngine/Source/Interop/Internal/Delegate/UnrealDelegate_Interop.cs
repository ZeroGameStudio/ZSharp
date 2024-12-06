// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class UnrealDelegate_Interop
{
	public static delegate* unmanaged<ZCallBuffer*, EZCallErrorCode> Execute;
	public static delegate* unmanaged<ConjugateHandle, GCHandle, ConjugateHandle> BindManagedDelegate;
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, char*, void> BindUnrealFunction;
	public static delegate* unmanaged<ConjugateHandle, void> Unbind;
	public static delegate* unmanaged<ConjugateHandle, uint8> IsBound;
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, uint8> IsBoundToObject;
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle> GetObject;
	public static delegate* unmanaged<ConjugateHandle, IntPtr, void> GetFunctionName;
}


