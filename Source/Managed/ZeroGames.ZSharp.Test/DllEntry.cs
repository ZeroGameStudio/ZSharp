// Copyright Zero Games. All Rights Reserved.

using ZeroGames.ZSharp.Core;

[assembly: DllEntry(typeof(ZeroGames.ZSharp.Test.DllEntry))]

namespace ZeroGames.ZSharp.Test;

internal static class DllEntry
{
    [DllMain]
    private static void DllMain(string[] args)
    {
        Logger.Log(args[0], args[1]);
    }
}


