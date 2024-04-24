// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

internal static class CLR_Interop
{

    [UnmanagedCallersOnly]
    public static void CollectGarbage(int32 generation, uint8 bAggressive, uint8 bBlocking, uint8 bCompacting)
    {
        Logger.Log("Request GC by unmanaged code.");
        //GC.Collect(generation, bAggressive > 0 ? GCCollectionMode.Aggressive : GCCollectionMode.Default, bBlocking > 0, bCompacting > 0);
        GC.Collect();
    }
    
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


