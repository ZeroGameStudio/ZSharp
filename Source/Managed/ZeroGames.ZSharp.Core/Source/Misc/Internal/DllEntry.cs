// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

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
        public uint8* GIsServerPtr;
        public uint8* GIsClientPtr;
        public uint8* GIsEditorPtr;
        public uint64* GFrameCounterPtr;
        public void* GConfig;
    }
    
    [StructLayout(LayoutKind.Sequential)]
    private unsafe struct Args
    {
        public UnmanagedProperties UnmanagedProperties;
        public UnmanagedFunctions UnmanagedFunctions;
        public void*** ManagedFunctions;
    }

    [UnmanagedCallersOnly]
    private static unsafe void DllMain(Args* args)
    {
        CoreGlobals_Interop.IsInGameThreadFuncPtr = (delegate* unmanaged<uint8>)args->UnmanagedProperties.IsInGameThreadFuncPtr;
        CoreGlobals_Interop.GIsServerPtr = args->UnmanagedProperties.GIsServerPtr;
        CoreGlobals_Interop.GIsClientPtr = args->UnmanagedProperties.GIsClientPtr;
        CoreGlobals_Interop.GIsEditorPtr = args->UnmanagedProperties.GIsEditorPtr;
        CoreGlobals_Interop.GFrameCounterPtr = args->UnmanagedProperties.GFrameCounterPtr;
        GConfig = new Config((IntPtr)args->UnmanagedProperties.GConfig);

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
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<int32>)&MasterAssemblyLoadContext_Interop.Unload;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<char*, void*, ELoadAssemblyErrorCode>)&MasterAssemblyLoadContext_Interop.LoadAssembly;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<char*, char*, char*, void*, EInvokeMethodErrorCode>)&MasterAssemblyLoadContext_Interop.InvokeMethod;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<InteropRuntimeTypeUri*, InteropRuntimeTypeHandle>)&MasterAssemblyLoadContext_Interop.GetType;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<ZCallHandle, ZCallBuffer*, IntPtr, EZCallErrorCode>)&MasterAssemblyLoadContext_Interop.ZCall_Red;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<char*, ZCallHandle>)&MasterAssemblyLoadContext_Interop.GetZCallHandle_Red;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<IntPtr, InteropRuntimeTypeHandle, IntPtr>)&MasterAssemblyLoadContext_Interop.BuildConjugate_Red;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<IntPtr, void>)&MasterAssemblyLoadContext_Interop.ReleaseConjugate_Red;

        // SlimAssemblyLoadContext interop functions
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<GCHandle, int32>)&SlimAssemblyLoadContext_Interop.Unload;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<GCHandle, char*, void*, ELoadAssemblyErrorCode>)&SlimAssemblyLoadContext_Interop.LoadAssembly;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<GCHandle, char*, char*, char*, void*, EInvokeMethodErrorCode>)&SlimAssemblyLoadContext_Interop.InvokeMethod;
        
        UE_LOG(LogZSharpScriptCore, "===================== ZSharp Startup =====================");
    }

}


