// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

internal static class SlimAssemblyLoadContext_Interop
{

    [UnmanagedCallersOnly]
    public static int32 Unload(GCHandle handle)
    {
        try
        {
            if (handle.Target is SlimAssemblyLoadContext alc)
            {
                alc.Unload();
                return 0;
            }

            return 1;
        }
        catch (Exception ex)
        {
            UnhandledExceptionHelper.Guard(ex);
            return -1;
        }
    }

    [UnmanagedCallersOnly]
    public static unsafe ELoadAssemblyErrorCode LoadAssembly(GCHandle handle, char* assemblyName, void* args)
    {
        try
        {
            return Unsafe.As<SlimAssemblyLoadContext>(handle.Target)!.LoadAssembly(new(assemblyName), args, out _);
        }
        catch (Exception ex)
        {
            UnhandledExceptionHelper.Guard(ex);
            return ELoadAssemblyErrorCode.UnknownError;
        }
    }

    [UnmanagedCallersOnly]
    public static unsafe EInvokeMethodErrorCode InvokeMethod(GCHandle handle, char* assemblyName, char* typeName, char* methodName, void* args)
    {
        try
        {
            return Unsafe.As<SlimAssemblyLoadContext>(handle.Target)!.InvokeMethod(new(assemblyName), new(typeName), new(methodName), args);
        }
        catch (Exception ex)
        {
            UnhandledExceptionHelper.Guard(ex);
            return EInvokeMethodErrorCode.UnknownError;
        }
    }

}


