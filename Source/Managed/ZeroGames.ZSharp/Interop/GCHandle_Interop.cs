using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Interop;

internal static class GCHandle_Interop
{
    
    [UnmanagedCallersOnly]
    public static void Free(GCHandle handle)
    {
        handle.Free();
    }
    
}


