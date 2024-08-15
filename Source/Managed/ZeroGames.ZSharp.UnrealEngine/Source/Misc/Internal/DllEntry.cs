// Copyright Zero Games. All Rights Reserved.

[assembly: DllEntry(typeof(DllEntry))]

namespace ZeroGames.ZSharp.UnrealEngine;

internal static class DllEntry
{
    [DllMain]
    private static unsafe void DllMain(){}
}


