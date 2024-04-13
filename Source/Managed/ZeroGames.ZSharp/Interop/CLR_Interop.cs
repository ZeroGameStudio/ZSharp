using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp;

internal static class CLR_Interop
{
    
    [UnmanagedCallersOnly]
    public static GCHandle CreateMasterALC()
    {
        MasterAssemblyLoadContext alc = new();
        
        Logger.Log($"Create Master ALC, handle: {GCHandle.ToIntPtr(alc.GCHandle)}");
        
        return alc.GCHandle;
    }
    
}


