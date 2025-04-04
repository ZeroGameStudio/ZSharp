﻿// Copyright Zero Games. All Rights Reserved.

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class EnhancedInputComponent_Interop
{
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, int64, GCHandle, ConjugateHandle> BindStatelessAction;
	public static delegate* unmanaged<ConjugateHandle, ConjugateHandle, int64, GCHandle, GCHandle, ConjugateHandle> BindStatefulAction;
	public static delegate* unmanaged[SuppressGCTransition]<ConjugateHandle, ConjugateHandle, uint8> RemoveBinding;
}


