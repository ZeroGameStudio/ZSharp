// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Runtime.Loader;
using System.Threading;
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
        
        // Trigger GameThreadScheduler static constructor.
        _ = GameThreadScheduler.Instance;
        
        // Inject GameThreadScheduler to ZeroGames.ZSharp.Core.
        Assembly coreAssembly = AssemblyLoadContext.Default.Assemblies.Single(asm => asm.GetName().Name == "ZeroGames.ZSharp.Core");
        Type schedulerType = coreAssembly.GetType("ZeroGames.ZSharp.Core.GameThreadScheduler")!;
        FieldInfo postField = schedulerType.GetField("_post", BindingFlags.Static | BindingFlags.NonPublic)!;
        postField.SetValue(null, static (SendOrPostCallback d, object? state) => { GameThreadScheduler.Instance.Post(d, state); });
        
        // Setup Z# SynchronizationContext if needed.
        if (AsyncSettings.UseZSharpSynchronizationContext)
        {
            check(SynchronizationContext.Current is null);
            SynchronizationContext.SetSynchronizationContext(new ZSharpSynchronizationContext());
        }

        // Catch unobserved exception from TaskScheduler.
        TaskScheduler.UnobservedTaskException += static (_, eventArgs) =>
        {
            GameThreadScheduler.Instance.Send(static ex =>
            {
                UnhandledExceptionHelper.Guard(Unsafe.As<Exception>(ex!), "Unobserved exception from TaskScheduler detected.");
            }, eventArgs.Exception);
        };
    }

}


