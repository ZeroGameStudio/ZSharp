﻿// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.UnrealEngine;

internal static unsafe class UnrealEngine_Interop
{
    
    public static delegate* unmanaged<uint8, char*, void> Log = null;
    public static delegate* unmanaged<uint8> IsInGameThread = null;
    
}


