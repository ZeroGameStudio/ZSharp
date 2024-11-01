// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

internal static class Clr_Interop
{

    [UnmanagedCallersOnly]
    public static void CollectGarbage(int32 generation, uint8 bAggressive, uint8 bBlocking, uint8 bCompacting)
    {
        UE_LOG(LogZSharpScriptCore, "Manually call managed GC.");
        GC.Collect();
    }

    [UnmanagedCallersOnly]
    public static uint8 CreateMasterAlc()
    {
        MasterAssemblyLoadContext alc = MasterAssemblyLoadContext.Create(out var alreadyExists);
        UE_CWARNING(alreadyExists, LogZSharpScriptCore, "Master ALC already exists.");

        return (uint8)(alreadyExists ? 0 : 1);
    }

    [UnmanagedCallersOnly]
    public static unsafe GCHandle CreateSlimAlc(char* name)
    {
        string nameStr = new(name);
        SlimAssemblyLoadContext? alc = SlimAssemblyLoadContext.Create(nameStr);
        UE_CWARNING(alc is null, LogZSharpScriptCore, $"Slim ALC [{nameStr}] already exists.");
        
        return alc?.GCHandle ?? default;
    }

}


