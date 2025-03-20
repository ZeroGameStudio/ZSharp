// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class EnhancedInputComponent_Interop
{
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, int64, GCHandle, ConjugateHandle> BindStatelessAction;
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, int64, GCHandle, GCHandle, ConjugateHandle> BindStatefulAction;
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, uint8> RemoveBinding;
}


