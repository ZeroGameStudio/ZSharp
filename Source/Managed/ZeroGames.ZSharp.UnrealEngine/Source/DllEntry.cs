// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.InteropServices;
using ZeroGames.ZSharp.UnrealEngine.EventLoop;

[assembly: DllEntry(typeof(DllEntry))]

namespace ZeroGames.ZSharp.UnrealEngine;

internal static class DllEntry
{

    [StructLayout(LayoutKind.Sequential)]
    private struct Args
    {
        public unsafe void*** ManagedFunctions;
    }
    
    [DllMain]
    private static unsafe void DllMain(Args* args) => Uncaught.FatalIfUncaught(() =>
    {
        int32 offset = 0;
        
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<EEventLoopEventType, float, float, double, double, void>)&EventLoop_Interop.NotifyEvent;

        return 0;
    }, -1);
}


