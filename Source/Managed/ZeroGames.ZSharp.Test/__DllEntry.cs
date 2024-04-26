// Copyright Zero Games. All Rights Reserved.

using ZeroGames.ZSharp.Core;

namespace ZeroGames.ZSharp.Test;

internal static class __DllEntry
{
    public static unsafe void DllMain(CommonDllMainArgs* rawArgs)
    {
        string[] args = rawArgs->Parse();
    }
}


