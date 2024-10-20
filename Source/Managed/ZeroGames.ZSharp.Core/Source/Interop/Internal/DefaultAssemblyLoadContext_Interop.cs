// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

internal static class DefaultAssemblyLoadContext_Interop
{

    [UnmanagedCallersOnly]
    public static unsafe ELoadAssemblyErrorCode LoadAssembly(char* assemblyName, void* args) => Uncaught.ErrorIfUncaught(() =>
    {
        return DefaultAssemblyLoadContextProxy.Instance.LoadAssembly(new(assemblyName), args, out _);
    }, ELoadAssemblyErrorCode.UnknownError);

    [UnmanagedCallersOnly]
    public static unsafe EInvokeMethodErrorCode InvokeMethod(char* assemblyName, char* typeName, char* methodName, void* args) => Uncaught.ErrorIfUncaught(() =>
    {
        return DefaultAssemblyLoadContextProxy.Instance.InvokeMethod(new(assemblyName), new(typeName), new(methodName), args);
    }, EInvokeMethodErrorCode.UnknownError);

}


