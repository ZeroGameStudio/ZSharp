// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

public static class WeakReferenceMonitor
{

    public static void Monit(WeakReference weak, IntPtr handle)
    {
        // Task.Run(() =>
        // {
        //     while (weak.IsAlive)
        //     {
        //         Logger.Log($"Waiting object {handle} to be released...");
        //         Thread.Sleep(1000);
        //     }
        //     
        //     Logger.Log($"Object {handle} finally released!");
        // });
    }
    
}