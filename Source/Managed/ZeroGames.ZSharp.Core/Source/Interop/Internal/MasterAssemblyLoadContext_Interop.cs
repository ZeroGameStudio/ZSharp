﻿// Copyright Zero Games. All Rights Reserved.

using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

internal static unsafe class MasterAssemblyLoadContext_Interop
{
    
    [UnmanagedCallersOnly]
    public static uint8 Tick(float deltaTime) => Uncaught.ErrorIfUncaught<uint8>(() =>
    {
        MasterAssemblyLoadContext? alc = MasterAssemblyLoadContext.Get();
        if (alc is null)
        {
            return 0;
        }
        
        alc.Tick(deltaTime);
        return 1;
    }, default);

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
    public static ELoadAssemblyErrorCode LoadAssembly(uint8* buffer, int32 size, void* args) => Uncaught.ErrorIfUncaught(() =>
    {
        MasterAssemblyLoadContext? alc = MasterAssemblyLoadContext.Get();
        if (alc is null)
        {
            return ELoadAssemblyErrorCode.AlcUnavailable;
        }

        return alc.LoadAssembly(new UnmanagedMemoryStream(buffer, size), args);
    }, ELoadAssemblyErrorCode.UnknownError);

    [UnmanagedCallersOnly]
    public static InteropRuntimeTypeHandle GetType(InteropRuntimeTypeLocator* locator) => Uncaught.ErrorIfUncaught(() =>
    {
        MasterAssemblyLoadContext? alc = MasterAssemblyLoadContext.Get();
        if (alc is null)
        {
            return new InteropRuntimeTypeHandle();
        }

        RuntimeTypeLocator root = new(locator);
        return new InteropRuntimeTypeHandle(alc.GetType(ref root));
    }, default);

    [UnmanagedCallersOnly]
    public static EZCallErrorCode ZCall_Red(ZCallHandle handle, ZCallBuffer* buffer) => Uncaught.ErrorIfUncaught(() =>
    {
        MasterAssemblyLoadContext? alc = MasterAssemblyLoadContext.Get();
        if (alc is null)
        {
            return EZCallErrorCode.AlcUnavailable;
        }

        return alc.ZCall_Red(handle, buffer);
    }, EZCallErrorCode.UnknownError);
    
    [UnmanagedCallersOnly]
    public static ZCallHandle GetZCallHandle_Red(char* name) => Uncaught.ErrorIfUncaught(() =>
    {
        MasterAssemblyLoadContext? alc = MasterAssemblyLoadContext.Get();
        if (alc is null)
        {
            return new ZCallHandle();
        }

        return alc.GetZCallHandle_Red(new string(name));
    }, default);
    
    [UnmanagedCallersOnly]
    public static IntPtr BuildConjugate_Red(IntPtr unmanaged, InteropRuntimeTypeHandle type) => Uncaught.ErrorIfUncaught(() =>
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
    public static void ReleaseConjugate_Red(IntPtr unmanaged) => Uncaught.ErrorIfUncaught(() =>
    {
        MasterAssemblyLoadContext? alc = MasterAssemblyLoadContext.Get();
        if (alc is null)
        {
            return;
        }

        alc.ReleaseConjugate_Red(unmanaged);
    });
    
    public static delegate* unmanaged<ZCallHandle, ZCallBuffer*, int32> SZCall_Black = null;
    public static delegate* unmanaged<char*, ZCallHandle> SGetZCallHandle_Black = null;
    public static delegate* unmanaged<uint16, IntPtr, IntPtr> SBuildConjugate_Black = null;
    public static delegate* unmanaged<uint16, IntPtr, void> SReleaseConjugate_Black = null;

}

