// Copyright Zero Games. All Rights Reserved.

[assembly: DllEntry(typeof(ZeroGames.ZSharp.Build.DllEntry))]

namespace ZeroGames.ZSharp.Build;

internal static class DllEntry
{

    [DllMain]
    private static void DllMain(string[] args)
    {
        // Z# Synchronization Context doesn't allow blocking wait so we use an empty context.
        SynchronizationContext? prevSynchronizationContext = SynchronizationContext.Current;
        SynchronizationContext.SetSynchronizationContext(null);
        
        string result = new BuildEngine(args).RunAsync().Result;
        UE_LOG(LogTemp, result);
        
        SynchronizationContext.SetSynchronizationContext(prevSynchronizationContext);
    }

}


