﻿// Copyright Zero Games. All Rights Reserved.

using System.IO;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

internal static class SlimAssemblyLoadContext_Interop
{

    [UnmanagedCallersOnly]
    public static int32 Unload(GCHandle handle) => Uncaught.ErrorIfUncaught(() =>
    {
        if (handle.Target is SlimAssemblyLoadContext alc)
        {
            alc.Unload();
            return 0;
        }

        return 1;
    }, -1);

    [UnmanagedCallersOnly]
    public static unsafe ELoadAssemblyErrorCode LoadAssembly(GCHandle handle, uint8* buffer, int32 size, void* args) => Uncaught.ErrorIfUncaught(() =>
    {
        return Unsafe.As<SlimAssemblyLoadContext>(handle.Target)!.LoadAssembly(new UnmanagedMemoryStream(buffer, size), args);
    }, ELoadAssemblyErrorCode.UnknownError);

    [UnmanagedCallersOnly]
    public static unsafe ECallMethodErrorCode CallMethod(GCHandle handle, char* assemblyName, char* typeName, char* methodName, void* args) => Uncaught.ErrorIfUncaught(() =>
    {
        return Unsafe.As<SlimAssemblyLoadContext>(handle.Target)!.CallMethod(new(assemblyName), new(typeName), new(methodName), args);
    }, ECallMethodErrorCode.UnknownError);

}

