using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Interop;

internal static class CLR_Interop
{
    
    [UnmanagedCallersOnly]
    public static GCHandle CreateMasterALC()
    {
        MasterAssemblyLoadContext alc = new();
        return alc.GCHandle;
    }
    
}


