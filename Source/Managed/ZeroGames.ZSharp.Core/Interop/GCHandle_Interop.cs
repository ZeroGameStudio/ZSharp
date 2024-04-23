using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

internal static class GCHandle_Interop
{
    
    [UnmanagedCallersOnly]
    public static int32 Free(GCHandle handle)
    {
        handle.Free();
        return 0;
    }
    
}


