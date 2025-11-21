// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Threading;

namespace ZeroGames.ZSharp.Core;

internal static class DllEntry
{
    
    [StructLayout(LayoutKind.Sequential)]
    private unsafe struct UnmanagedFunction
    {
        public char* TypeName;
        public char* FieldName;
        public void* Address;
    }
    
    [StructLayout(LayoutKind.Sequential)]
    private unsafe struct UnmanagedFunctions
    {
        public int32 Count;
        public UnmanagedFunction* Functions;
    }
    
    [StructLayout(LayoutKind.Sequential)]
    private unsafe struct UnmanagedProperties
    {
        public void* IsInGameThreadFuncPtr;
    }
    
    [StructLayout(LayoutKind.Sequential)]
    private unsafe struct Args
    {
        public UnmanagedFunctions UnmanagedFunctions;
        public void*** ManagedFunctions;
        public UnmanagedProperties UnmanagedProperties;
        public uint8 WaitForDebugger;
    }

    [UnmanagedCallersOnly]
    private static unsafe void DllMain(Args* args)
    {
        AppDomain.CurrentDomain.UnhandledException += (_, eventArgs) =>
        {
            CoreLog.Error($"FATAL!!!!!!! MANAGED EXCEPTION UNHANDLED!!!!!!!{Environment.NewLine}{eventArgs.ExceptionObject}");
        };
        
        for (int32 i = 0; i < args->UnmanagedFunctions.Count; ++i)
        {
            UnmanagedFunction* function = args->UnmanagedFunctions.Functions + i;
            string typeName = new(function->TypeName);
            string fieldName = new(function->FieldName);
            InteropBindingHelper.GetStaticFunctionPointerField(typeName, fieldName).SetValue(null, (IntPtr)function->Address);
        }

        int32 offset = 0;
        // CLR interop functions
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<int32, uint8, uint8, uint8, void>)&Clr_Interop.CollectGarbage;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<uint8>)&Clr_Interop.CreateMasterAlc;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<char*, GCHandle>)&Clr_Interop.CreateSlimAlc;

        // GCHandle interop functions
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<GCHandle, int32>)&GCHandle_Interop.Free;

        // DefaultAssemblyLoadContext interop functions
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<char*, void*, ELoadAssemblyErrorCode>)&DefaultAssemblyLoadContext_Interop.LoadAssembly;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<char*, char*, char*, void*, EInvokeMethodErrorCode>)&DefaultAssemblyLoadContext_Interop.InvokeMethod;

        // MasterAssemblyLoadContext interop functions
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<int32>)&MasterAssemblyLoadContext_Interop.PrepareUnloading;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<int32>)&MasterAssemblyLoadContext_Interop.Unload;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<char*, void*, ELoadAssemblyErrorCode>)&MasterAssemblyLoadContext_Interop.LoadAssembly;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<char*, char*, char*, void*, EInvokeMethodErrorCode>)&MasterAssemblyLoadContext_Interop.InvokeMethod;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<InteropRuntimeTypeUri, InteropRuntimeTypeHandle>)&MasterAssemblyLoadContext_Interop.GetType;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<ZCallHandle, ZCallBuffer*, IntPtr, EZCallErrorCode>)&MasterAssemblyLoadContext_Interop.ZCall_Red;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<char*, ZCallHandle>)&MasterAssemblyLoadContext_Interop.GetZCallHandle_Red;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<IntPtr, InteropRuntimeTypeHandle, IntPtr>)&MasterAssemblyLoadContext_Interop.BuildConjugate_Red;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<IntPtr, void>)&MasterAssemblyLoadContext_Interop.ReleaseConjugate_Red;

        // SlimAssemblyLoadContext interop functions
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<GCHandle, int32>)&SlimAssemblyLoadContext_Interop.Unload;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<GCHandle, char*, void*, ELoadAssemblyErrorCode>)&SlimAssemblyLoadContext_Interop.LoadAssembly;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<GCHandle, char*, char*, char*, void*, EInvokeMethodErrorCode>)&SlimAssemblyLoadContext_Interop.InvokeMethod;
        
        GameThreadScheduler.IsInGameThreadFuncPtr = (delegate* unmanaged<uint8>)args->UnmanagedProperties.IsInGameThreadFuncPtr;

        if (args->WaitForDebugger != 0)
        {
            while (!Debugger.IsAttached)
            {
                CoreLog.Log("Waiting for debugger...");
                Thread.Sleep(TimeSpan.FromMilliseconds(1000));
            }
        }
        
        CoreLog.Log("===================== Z# Startup =====================");
    }

}


