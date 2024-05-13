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
    public static unsafe void LoadAssembly(uint8* buffer, int32 size, void* args) => Uncaught.ErrorIfUncaught(() =>
    {
        MasterAssemblyLoadContext? alc = MasterAssemblyLoadContext.Get();
        if (alc is null)
        {
            return;
        }

        Assembly asm = alc.LoadFromStream(new UnmanagedMemoryStream(buffer, size));
        DllMainStatics.TryInvokeDllMain(asm, args, out var res);
    });

    [UnmanagedCallersOnly]
    public static unsafe InteropRuntimeTypeHandle GetType(char* assemblyName, char* typeName) => Uncaught.ErrorIfUncaught(() =>
    {
        MasterAssemblyLoadContext? alc = MasterAssemblyLoadContext.Get();
        if (alc is null)
        {
            return new InteropRuntimeTypeHandle();
        }

        Assembly? asm = alc.Assemblies.FirstOrDefault(asm => asm.GetName().Name == new string(assemblyName));
        if (asm is null)
        {
            return new InteropRuntimeTypeHandle();
        }

        Type? type = asm.GetType(new string(typeName));
        return new InteropRuntimeTypeHandle(type);
    }, default);

    [UnmanagedCallersOnly]
    public static unsafe int32 ZCall_Red(ZCallHandle handle, ZCallBuffer* buffer) => Uncaught.ErrorIfUncaught(() =>
    {
        MasterAssemblyLoadContext? alc = MasterAssemblyLoadContext.Get();
        if (alc is null)
        {
            return 1;
        }

        return alc.ZCall_Red(handle, buffer);
    }, -1);
    
    [UnmanagedCallersOnly]
    public static unsafe ZCallHandle GetZCallHandle_Red(char* name) => Uncaught.ErrorIfUncaught(() =>
    {
        MasterAssemblyLoadContext? alc = MasterAssemblyLoadContext.Get();
        if (alc is null)
        {
            return new ZCallHandle();
        }

        return alc.GetZCallHandle_Red(new string(name));
    }, default);
    
    [UnmanagedCallersOnly]
    public static unsafe IntPtr BuildConjugate_Red(IntPtr unmanaged, InteropRuntimeTypeHandle type) => Uncaught.ErrorIfUncaught(() =>
    {
        MasterAssemblyLoadContext? alc = MasterAssemblyLoadContext.Get();
        if (alc is null)
        {
            return default;
        }

        Type? managedType = type.Type;
        if (managedType is null)
        {
            return default;
        }

        return alc.BuildConjugate_Red(unmanaged, managedType);
    }, default);
    
    [UnmanagedCallersOnly]
    public static unsafe void ReleaseConjugate_Red(IntPtr unmanaged) => Uncaught.ErrorIfUncaught(() =>
    {
        MasterAssemblyLoadContext? alc = MasterAssemblyLoadContext.Get();
        if (alc is null)
        {
            return;
        }

        alc.ReleaseConjugate_Red(unmanaged);
    });
    
    public static unsafe delegate* unmanaged<ZCallHandle, ZCallBuffer*, int32> SZCall_Black = null;
    public static unsafe delegate* unmanaged<char*, ZCallHandle> SGetZCallHandle_Black = null;
    public static unsafe delegate* unmanaged<uint16, IntPtr> SBuildConjugate_Black = null;
    public static unsafe delegate* unmanaged<uint16, IntPtr, void> SReleaseConjugate_Black = null;

}


