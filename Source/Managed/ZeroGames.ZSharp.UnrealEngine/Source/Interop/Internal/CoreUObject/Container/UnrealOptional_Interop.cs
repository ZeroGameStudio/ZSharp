﻿// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine;

internal static unsafe class UnrealOptional_Interop
{
	public static delegate* unmanaged<ConjugateHandle, ref ZCallBufferSlot, uint8> Get;
	public static delegate* unmanaged<ConjugateHandle, ZCallBufferSlot, void> Set;
	public static delegate* unmanaged<ConjugateHandle, void> Reset;
	public static delegate* unmanaged<ConjugateHandle, uint8> IsSet;
}

