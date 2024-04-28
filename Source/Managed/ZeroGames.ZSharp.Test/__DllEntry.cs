// Copyright Zero Games. All Rights Reserved.

using ZeroGames.ZSharp.Core;

namespace ZeroGames.ZSharp.Test;

internal static class __DllEntry
{
    public static void DllMain(string[] args)
    {
        Logger.Log(args[0], args[1]);
    }
}


