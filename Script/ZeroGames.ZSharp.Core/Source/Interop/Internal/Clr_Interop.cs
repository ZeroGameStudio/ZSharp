// Copyright Zero Games. All Rights Reserved.

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

internal static class Clr_Interop
{

    [UnmanagedCallersOnly]
    public static void CollectGarbage(int32 generation, uint8 bAggressive, uint8 bBlocking, uint8 bCompacting)
    {
        CoreLog.Log("Manually call managed GC.");
        GC.Collect();
    }

    [UnmanagedCallersOnly]
    public static uint8 CreateMasterAlc()
    {
        MasterAssemblyLoadContext alc = MasterAssemblyLoadContext.Create(out var alreadyExists);

        return (uint8)(alreadyExists ? 0 : 1);
    }

    [UnmanagedCallersOnly]
    public static unsafe GCHandle CreateSlimAlc(char* name)
    {
        string nameStr = new(name);
        SlimAssemblyLoadContext? alc = SlimAssemblyLoadContext.Create(nameStr);

        return alc?.GCHandle ?? default;
    }

}


