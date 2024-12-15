// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class EnhancedInputComponent_Interop
{
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, int64, GCHandle, ConjugateHandle> BindAction;
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, uint8> RemoveBinding;
}


