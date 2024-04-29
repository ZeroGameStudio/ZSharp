// Copyright Zero Games. All Rights Reserved.

using ZeroGames.ZSharp.Core;

[assembly: DllEntry(typeof(ZeroGames.ZSharp.UnrealEngine.DllEntry))]

namespace ZeroGames.ZSharp.UnrealEngine;

internal static class DllEntry
{
    [DllMain]
    private static void DllMain()
    {
        Logger.Log("===================== UnrealEngine Assembly Loaded =====================");
    }
}


