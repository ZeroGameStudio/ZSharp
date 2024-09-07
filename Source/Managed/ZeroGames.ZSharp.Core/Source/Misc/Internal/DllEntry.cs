// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
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
    private unsafe struct Args
    {
        public UnmanagedFunctions UnmanagedFunctions;
        public void*** ManagedFunctions;
    }

    [UnmanagedCallersOnly]
    private static unsafe void DllMain(Args* args) => Uncaught.FatalIfUncaught(() =>
    {
        for (int32 i = 0; i < args->UnmanagedFunctions.Count; ++i)
        {
            UnmanagedFunction* function = args->UnmanagedFunctions.Functions + i;
            string typeName = new(function->TypeName);
            Type? type = Assembly.GetExecutingAssembly().GetType(typeName);
            if (type is null)
            {
                throw new Exception($"Binding failed, type not found. {typeName}");
            }

            string fieldName = new(function->FieldName);
            FieldInfo? field = type.GetField(fieldName);
            if (field is null)
            {
                throw new Exception($"Binding failed, field not found. {typeName}.{fieldName}");
            }

            if (!field.IsStatic)
            {
                throw new Exception($"Binding failed, field is not static. {typeName}.{fieldName}");
            }

            if (!field.FieldType.IsUnmanagedFunctionPointer)
            {
                throw new Exception(
                    $"Binding failed, field is not unmanaged function pointer. {typeName}.{fieldName}");
            }

            field.SetValue(null, (IntPtr)function->Address);
        }

        int32 offset = 0;
        // CLR interop functions
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<int32, uint8, uint8, uint8, void>)&Clr_Interop.CollectGarbage;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<GCHandle>)&Clr_Interop.CreateMasterAlc;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<char*, GCHandle>)&Clr_Interop.CreateSlimAlc;

        // GCHandle interop functions
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<GCHandle, int32>)&GCHandle_Interop.Free;

        // MasterAssemblyLoadContext interop functions
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<float, uint8>)&MasterAssemblyLoadContext_Interop.Tick;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<int32>)&MasterAssemblyLoadContext_Interop.Unload;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<char*, void*, ELoadAssemblyErrorCode>)&MasterAssemblyLoadContext_Interop.LoadAssembly;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<char*, char*, char*, void*, EInvokeMethodErrorCode>)&MasterAssemblyLoadContext_Interop.InvokeMethod;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<InteropRuntimeTypeLocator*, InteropRuntimeTypeHandle>)&MasterAssemblyLoadContext_Interop.GetType;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<ZCallHandle, ZCallBuffer*, EZCallErrorCode>)&MasterAssemblyLoadContext_Interop.ZCall_Red;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<char*, ZCallHandle>)&MasterAssemblyLoadContext_Interop.GetZCallHandle_Red;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<IntPtr, InteropRuntimeTypeHandle, IntPtr>)&MasterAssemblyLoadContext_Interop.BuildConjugate_Red;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<IntPtr, void>)&MasterAssemblyLoadContext_Interop.ReleaseConjugate_Red;
        
        // SlimAssemblyLoadContext interop functions
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<GCHandle, int32>)&SlimAssemblyLoadContext_Interop.Unload;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<GCHandle, char*, void*, ELoadAssemblyErrorCode>)&SlimAssemblyLoadContext_Interop.LoadAssembly;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<GCHandle, char*, char*, char*, void*, EInvokeMethodErrorCode>)&SlimAssemblyLoadContext_Interop.InvokeMethod;

        Logger.Log("===================== ZSharp Startup =====================");
    });
    
}


