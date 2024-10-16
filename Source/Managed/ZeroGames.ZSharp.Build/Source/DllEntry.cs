﻿// Copyright Zero Games. All Rights Reserved.

[assembly: DllEntry(typeof(ZeroGames.ZSharp.Build.DllEntry))]

namespace ZeroGames.ZSharp.Build;

internal static class DllEntry
{

    [DllMain]
    private static void DllMain(string[] args) => Logger.Log(new BuildEngine(args).RunAsync().Result);

}


