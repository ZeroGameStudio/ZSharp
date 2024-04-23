using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

internal static class CLR_Interop
{
    
    [UnmanagedCallersOnly]
    public static GCHandle CreateMasterALC()
    {
        try
        {
            MasterAssemblyLoadContext alc = MasterAssemblyLoadContext.Create();

            Logger.Log($"Create Master ALC, name: {alc.Name}, handle: {GCHandle.ToIntPtr(alc.GCHandle)}");

            return alc.GCHandle;
        }
        catch (Exception ex)
        {
            Logger.Log(ex.StackTrace);
            return new();
        }
    }
    
    [UnmanagedCallersOnly]
    public static unsafe GCHandle CreateSlimALC(char* name)
    {
        try
        {
            SlimAssemblyLoadContext alc = SlimAssemblyLoadContext.Create(new string(name));

            Logger.Log($"Create Slim ALC, name: {alc.Name}, handle: {GCHandle.ToIntPtr(alc.GCHandle)}");

            return alc.GCHandle;
        }
        catch (Exception ex)
        {
            Logger.Log(ex.StackTrace);
            return new();
        }
    }
    
}


