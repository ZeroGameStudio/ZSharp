// Copyright Zero Games. All Rights Reserved.

using ZeroGames.ZSharp.Core;
using ZeroGames.ZSharp.UnrealEngine.Core;

[assembly: DllEntry(typeof(ZeroGames.ZSharp.Test.DllEntry))]

namespace ZeroGames.ZSharp.Test;

public class A
{
    public A(int32 t = 6)
    {
        Logger.Log(t);
    }
}

internal static class DllEntry
{
    [DllMain]
    private static void DllMain(string[] args)
    {

    }
}


