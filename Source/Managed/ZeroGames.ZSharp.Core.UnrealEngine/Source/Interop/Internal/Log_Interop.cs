// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.UnrealEngine;

internal static unsafe class Log_Interop
{
    public static delegate* unmanaged<char*, ELogVerbosity, char*, void> Log = null;
}


