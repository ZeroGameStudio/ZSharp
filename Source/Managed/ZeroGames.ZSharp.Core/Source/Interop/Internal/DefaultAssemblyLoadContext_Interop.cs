// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

internal static class DefaultAssemblyLoadContext_Interop
{

    [UnmanagedCallersOnly]
    public static unsafe ELoadAssemblyErrorCode LoadAssembly(char* assemblyName, void* args)
    {
        try
        {
            return DefaultAssemblyLoadContextProxy.Instance.LoadAssembly(new(assemblyName), args, out _);
        }
        catch (Exception ex)
        {
            UnhandledExceptionHelper.Guard(ex);
            return ELoadAssemblyErrorCode.UnknownError;
        }
    }

    [UnmanagedCallersOnly]
    public static unsafe EInvokeMethodErrorCode InvokeMethod(char* assemblyName, char* typeName, char* methodName, void* args)
    {
        try
        {
            return DefaultAssemblyLoadContextProxy.Instance.InvokeMethod(new(assemblyName), new(typeName), new(methodName), args);
        }
        catch (Exception ex)
        {
            UnhandledExceptionHelper.Guard(ex);
            return EInvokeMethodErrorCode.UnknownError;
        }
    }

}


