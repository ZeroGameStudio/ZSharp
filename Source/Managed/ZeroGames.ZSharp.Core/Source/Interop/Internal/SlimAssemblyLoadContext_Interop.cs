// Copyright Zero Games. All Rights Reserved.

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
    public static unsafe ELoadAssemblyErrorCode LoadAssembly(GCHandle handle, char* assemblyName, void* args) => Uncaught.ErrorIfUncaught(() =>
    {
        return Unsafe.As<SlimAssemblyLoadContext>(handle.Target)!.LoadAssembly(new(assemblyName), args, out _);
    }, ELoadAssemblyErrorCode.UnknownError);

    [UnmanagedCallersOnly]
    public static unsafe EInvokeMethodErrorCode InvokeMethod(GCHandle handle, char* assemblyName, char* typeName, char* methodName, void* args) => Uncaught.ErrorIfUncaught(() =>
    {
        return Unsafe.As<SlimAssemblyLoadContext>(handle.Target)!.InvokeMethod(new(assemblyName), new(typeName), new(methodName), args);
    }, EInvokeMethodErrorCode.UnknownError);

}


