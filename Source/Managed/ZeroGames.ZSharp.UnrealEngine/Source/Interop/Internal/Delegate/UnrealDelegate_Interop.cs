// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine;

public static unsafe class UnrealDelegate_Interop
{
	public static delegate* unmanaged<ZCallBuffer*, EZCallErrorCode> Execute;
	public static delegate* unmanaged<ConjugateHandle, GCHandle, ConjugateHandle> BindManagedDelegate;
	internal static delegate* unmanaged<ConjugateHandle, ConjugateHandle, char*, void> BindUnrealFunction;
	internal static delegate* unmanaged<ConjugateHandle, void> Unbind;
	internal static delegate* unmanaged<ConjugateHandle, uint8> IsBound;
	internal static delegate* unmanaged<ConjugateHandle, ConjugateHandle, uint8> IsBoundToObject;
	internal static delegate* unmanaged<ConjugateHandle, ConjugateHandle> GetObject;
	internal static delegate* unmanaged<ConjugateHandle, IntPtr, void> GetFunctionName;
}


