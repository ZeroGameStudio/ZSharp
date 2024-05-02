// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

internal static class Clr_Interop
{

    [UnmanagedCallersOnly]
    public static void CollectGarbage(int32 generation, uint8 bAggressive, uint8 bBlocking, uint8 bCompacting) => Uncaught.ErrorIfUncaught(() =>
    {
        GC.Collect();
    });

    [UnmanagedCallersOnly]
    public static GCHandle CreateMasterAlc() => Uncaught.ErrorIfUncaught(() =>
    {
        MasterAssemblyLoadContext alc = MasterAssemblyLoadContext.Create();

        return alc.GCHandle;
    }, default);

    [UnmanagedCallersOnly]
    public static unsafe GCHandle CreateSlimAlc(char* name) => Uncaught.ErrorIfUncaught(() =>
    {
        SlimAssemblyLoadContext alc = SlimAssemblyLoadContext.Create(new string(name));

        return alc.GCHandle;
    }, default);

}


