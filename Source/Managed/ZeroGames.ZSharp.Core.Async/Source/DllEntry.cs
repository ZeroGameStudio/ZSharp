// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;
using ZeroGames.ZSharp.Async;

namespace ZeroGames.ZSharp.Core.Async;

internal static class DllEntry
{

    [StructLayout(LayoutKind.Sequential)]
    private struct Args
    {
        public unsafe void*** ManagedFunctions;
    }
    
    [UnmanagedCallersOnly]
    private static unsafe void DllMain(Args* args) => Uncaught.FatalIfUncaught(() =>
    {
        int32 offset = 0;
        
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<EEventLoopTickingGroup, float, float, double, double, void>)&EventLoop_Interop.NotifyEvent;

        return 0;
    }, -1);

}


