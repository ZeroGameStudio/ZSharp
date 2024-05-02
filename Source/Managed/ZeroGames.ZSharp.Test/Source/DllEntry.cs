// Copyright Zero Games. All Rights Reserved.

using ZeroGames.ZSharp.Core;
using ZeroGames.ZSharp.UnrealEngine.Core;

[assembly: DllEntry(typeof(ZeroGames.ZSharp.Test.DllEntry))]

namespace ZeroGames.ZSharp.Test;

public class A
{
    public int32 X { get; init; }
}

internal static class DllEntry
{
    [DllMain]
    private static void DllMain(string[] args)
    {
        Logger.Error(Path.Combine("Intermediate/ZSharp/ProjectFiles", "ZeroGames.ZSharp.Build"));
    }
}


