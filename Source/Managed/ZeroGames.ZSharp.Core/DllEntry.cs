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
    private static unsafe int32 DllMain(Args* args)
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
                throw new Exception($"Binding failed, field is not unmanaged function pointer. {typeName}.{fieldName}");
            }
            
            field.SetValue(null, (IntPtr)function->Address);
        }

        int32 offset = 0;
        // CLR interop functions
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<int32, uint8, uint8, uint8, void>)&CLR_Interop.CollectGarbage;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<GCHandle>)&CLR_Interop.CreateMasterALC;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<char*, GCHandle>)&CLR_Interop.CreateSlimALC;
        
        // GCHandle interop functions
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<GCHandle, int32>)&GCHandle_Interop.Free;

        // MasterAssemblyLoadContext interop functions
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<int32>)&MasterAssemblyLoadContext_Interop.Unload;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<uint8*, int32, void*, GCHandle>)&MasterAssemblyLoadContext_Interop.LoadAssembly;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<ConjugateHandle, int32>)&MasterAssemblyLoadContext_Interop.ReleaseConjugate;

        // SlimAssemblyLoadContext interop functions
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<GCHandle, int32>)&SlimAssemblyLoadContext_Interop.Unload;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<GCHandle, uint8*, int32, void*, int32>)&SlimAssemblyLoadContext_Interop.LoadAssembly;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<GCHandle, char*, char*, char*, void*, int32>)&SlimAssemblyLoadContext_Interop.CallMethod;
        
        // Assembly interop functions
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<GCHandle, IntPtr, int32>)&Assembly_Interop.GetName;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<GCHandle, char*, GCHandle>)&Assembly_Interop.GetType;
        
        // Type interop functions
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<GCHandle, IntPtr, int32>)&Type_Interop.GetName;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<GCHandle, IntPtr, ConjugateHandle>)&Type_Interop.BuildConjugate;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<GCHandle, char*, GCHandle>)&Type_Interop.GetMethodInfo;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<GCHandle, char*, GCHandle>)&Type_Interop.GetPropertyInfo;
        
        // MethodInfo interop functions
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<GCHandle, IntPtr, int32>)&MethodInfo_Interop.GetName;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<GCHandle, int32>)&MethodInfo_Interop.GetNumSlots;
        *args->ManagedFunctions[offset++] = (delegate* unmanaged<GCHandle, ZCallBuffer*, int32>)&MethodInfo_Interop.Invoke;
        
        Logger.Log("===================== ZSharp Startup =====================");
        
        return 0;
    }
    
}


