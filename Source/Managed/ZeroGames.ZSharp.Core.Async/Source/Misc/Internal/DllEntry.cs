// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Threading.Tasks;

namespace ZeroGames.ZSharp.Core.Async;

internal static class DllEntry
{

    [StructLayout(LayoutKind.Sequential)]
    private struct Args
    {
        public unsafe void*** ManagedFunctions;
    }
    
    [UnmanagedCallersOnly]
    private static unsafe void DllMain(Args* args)
    {
        int32 offset = 0;
        
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<EEventLoopTickingGroup, float, float, double, double, IntPtr, void>)&EventLoop_Interop.NotifyEvent;
        
        // Trigger Z# Synchronization Context initialization.
        _ = ZSharpSynchronizationContext.Instance;
        
        // Catch unobserved exception from TaskScheduler.
        TaskScheduler.UnobservedTaskException += static (_, eventArgs) =>
        {
            ZSharpSynchronizationContext.Instance.Send(static ex =>
            {
                UnhandledExceptionHelper.Guard(Unsafe.As<Exception>(ex!), "Unobserved exception from TaskScheduler detected.");
            }, eventArgs.Exception);
        };
    }

}


