// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

internal static class MasterAssemblyLoadContext_Interop
{

    [UnmanagedCallersOnly]
    public static int32 Unload() => Uncaught.ErrorIfUncaught(() =>
    {
        MasterAssemblyLoadContext? alc = MasterAssemblyLoadContext.Get();
        if (alc is null)
        {
            return -1;
        }

        alc.Unload();
        return 0;
    }, -1);

    [UnmanagedCallersOnly]
    public static unsafe GCHandle LoadAssembly(uint8* buffer, int32 size, void* args) => Uncaught.ErrorIfUncaught(() =>
    {
        MasterAssemblyLoadContext? alc = MasterAssemblyLoadContext.Get();
        if (alc is null)
        {
            return new();
        }

        Assembly asm = alc.LoadFromStream(new UnmanagedMemoryStream(buffer, size));
        DllMainStatics.TryInvokeDllMain(asm, args, out var res);

        return GCHandle.Alloc(asm);
    }, default);

    [UnmanagedCallersOnly]
    public static int32 ReleaseConjugate(ConjugateHandle handle) => Uncaught.ErrorIfUncaught(() =>
    {
        if (handle.ToGCHandle().Target is IConjugate conjugate)
        {
            conjugate.ReleaseConjugate();

            return 0;
        }

        return -1;
    }, -1);
    
    public static unsafe delegate* unmanaged<ZCallHandle, ZCallBuffer*, int32> SZCallByHandle = null;
    public static unsafe delegate* unmanaged<char*, ZCallBuffer*, ZCallHandle*, int32> SZCallByName = null;
    public static unsafe delegate* unmanaged<ZCallHandle, ZCallBuffer*, int32, void> SZCallByHandle_AnyThread = null;
    public static unsafe delegate* unmanaged<char*, ZCallHandle> SGetZCallHandle = null;
    
}


