// Copyright Zero Games. All Rights Reserved.

using ZeroGames.ZSharp.Core;

namespace ZeroGames.ZSharp.GlueGenerator;

internal static class __DllEntry
{

    public static unsafe int32 DllMain(CommonDllMainArgs* rawArgs)
    {
        string[] args = rawArgs->ToStringArray();
        
        Logger.Log("===================== Glue Generator =====================");
        Logger.Log($"argc: {args.Length}");
        for (int32 i = 0; i < args.Length; ++i)
        {
            Logger.Log($"arg{i}: {args[i]}");
        }
        
        return 0;
    }
    
}


