// Copyright Zero Games. All Rights Reserved.

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

namespace ZeroGames.ZSharp.Core;

internal static unsafe class CoreLog_Interop
{
    public static delegate* unmanaged[SuppressGCTransition]<uint8, char*, void> Log;
}


