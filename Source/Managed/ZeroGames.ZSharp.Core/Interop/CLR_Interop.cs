// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

internal static class CLR_Interop
{

    [UnmanagedCallersOnly]
    public static void CollectGarbage(int32 generation, uint8 bAggressive, uint8 bBlocking, uint8 bCompacting)
    {
        GC.Collect();
    }
    
    [UnmanagedCallersOnly]
    public static GCHandle CreateMasterALC()
    {
        MasterAssemblyLoadContext alc = MasterAssemblyLoadContext.Create();

        return alc.GCHandle;
    }
    
    [UnmanagedCallersOnly]
    public static unsafe GCHandle CreateSlimALC(char* name)
    {
        SlimAssemblyLoadContext alc = SlimAssemblyLoadContext.Create(new string(name));

        return alc.GCHandle;
    }
    
}


