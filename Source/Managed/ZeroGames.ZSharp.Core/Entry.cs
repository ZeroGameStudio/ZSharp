using System.Diagnostics;
using System.Reflection;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

[StructLayout(LayoutKind.Sequential)]
internal unsafe struct UnmanagedFunctions
{
    public delegate* unmanaged<uint8, char*, void> UE_Log;
    
    public delegate* unmanaged<char*, IntPtr> InteropString_Alloc;
    public delegate* unmanaged<IntPtr, void> InteropString_Free;
    public delegate* unmanaged<IntPtr, char*> InteropString_GetData;
    public delegate* unmanaged<IntPtr, char*, void> InteropString_SetData;

    public delegate* unmanaged<ZCallHandle, ZCallBuffer*, int32> MasterAssemblyLoadContext_ZCallByHandle;
    public delegate* unmanaged<char*, ZCallBuffer*, ZCallHandle*, uint8, int32> MasterAssemblyLoadContext_ZCallByName;
    public delegate* unmanaged<char*, ZCallHandle> MasterAssemblyLoadContext_GetZCallHandle;
}

[StructLayout(LayoutKind.Sequential)]
internal struct StartupInput
{
    public UnmanagedFunctions UnmanagedFunctions;
}

[StructLayout(LayoutKind.Sequential)]
internal unsafe struct ManagedFunctions
{
    public delegate* unmanaged<GCHandle> CLR_CreateMasterALC;
    
    public delegate* unmanaged<GCHandle, void> GCHandle_Free;

    public delegate* unmanaged<void> MasterAssemblyLoadContext_Unload;
    public delegate* unmanaged<uint8*, int32, void*, GCHandle> MasterAssemblyLoadContext_LoadAssembly;
    
    public delegate* unmanaged<GCHandle, IntPtr, uint8> Assembly_GetName;
    public delegate* unmanaged<GCHandle, char*, GCHandle> Assembly_GetType;
    
    public delegate* unmanaged<GCHandle, IntPtr, uint8> Type_GetName;
    public delegate* unmanaged<GCHandle, IntPtr, ConjugateHandle> Type_BuildConjugate;
    public delegate* unmanaged<GCHandle, char*, GCHandle> Type_GetMethodInfo;
    public delegate* unmanaged<GCHandle, char*, GCHandle> Type_GetPropertyInfo;
    
    public delegate* unmanaged<GCHandle, IntPtr, uint8> MethodInfo_GetName;
    public delegate* unmanaged<GCHandle, ZCallBuffer*, int32> MethodInfo_Invoke;
}

[StructLayout(LayoutKind.Sequential)]
internal struct StartupOutput
{
    public ManagedFunctions ManagedFunctions;
}

internal static class Entry
{
    [UnmanagedCallersOnly]
    private static unsafe int DllMain(StartupInput* input, StartupOutput* output)
    {
        // UE
        UE_Interop.GUE_Log = input->UnmanagedFunctions.UE_Log;
        
        // InteropString
        InteropString_Interop.GAlloc = input->UnmanagedFunctions.InteropString_Alloc;
        InteropString_Interop.GFree = input->UnmanagedFunctions.InteropString_Free;
        InteropString_Interop.GGetData = input->UnmanagedFunctions.InteropString_GetData;
        InteropString_Interop.GSetData = input->UnmanagedFunctions.InteropString_SetData;
        
        // MasterAssemblyLoadContext
        MasterAssemblyLoadContext_Interop.GZCallByHandle = input->UnmanagedFunctions.MasterAssemblyLoadContext_ZCallByHandle;
        MasterAssemblyLoadContext_Interop.GZCallByName = input->UnmanagedFunctions.MasterAssemblyLoadContext_ZCallByName;
        MasterAssemblyLoadContext_Interop.GGetZCallHandle = input->UnmanagedFunctions.MasterAssemblyLoadContext_GetZCallHandle;
        
        // CLR interop functions
        output->ManagedFunctions.CLR_CreateMasterALC = &CLR_Interop.CreateMasterALC;
        
        // GCHandle interop functions
        output->ManagedFunctions.GCHandle_Free = &GCHandle_Interop.Free;

        // MasterAssemblyLoadContext interop functions
        output->ManagedFunctions.MasterAssemblyLoadContext_Unload = &MasterAssemblyLoadContext_Interop.Unload;
        output->ManagedFunctions.MasterAssemblyLoadContext_LoadAssembly = &MasterAssemblyLoadContext_Interop.LoadAssembly;

        // Assembly interop functions
        output->ManagedFunctions.Assembly_GetName = &Assembly_Interop.GetName;
        output->ManagedFunctions.Assembly_GetType = &Assembly_Interop.GetType;
        
        // Type interop functions
        output->ManagedFunctions.Type_GetName = &Type_Interop.GetName;
        output->ManagedFunctions.Type_BuildConjugate = &Type_Interop.BuildConjugate;
        output->ManagedFunctions.Type_GetMethodInfo = &Type_Interop.GetMethodInfo;
        output->ManagedFunctions.Type_GetPropertyInfo = &Type_Interop.GetPropertyInfo;
        
        // MethodInfo interop functions
        output->ManagedFunctions.MethodInfo_GetName = &MethodInfo_Interop.GetName;
        output->ManagedFunctions.MethodInfo_Invoke = &MethodInfo_Interop.Invoke;
        
        Logger.Log("====================== CoreCLR Startup ======================");
        
        // while (!Debugger.IsAttached)
        // {
        //     Logger.Log("Waiting for debugger...");
        // }
        
        return 1;
    }
}


