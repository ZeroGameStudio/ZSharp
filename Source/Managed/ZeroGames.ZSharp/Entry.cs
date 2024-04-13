using System.Reflection;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp;

[StructLayout(LayoutKind.Sequential)]
internal unsafe struct UnmanagedFunctions
{
    public delegate* unmanaged<uint8, char*, void> UE_Log;
    
    public delegate* unmanaged<char*, IntPtr> InteropString_Alloc;
    public delegate* unmanaged<IntPtr, void> InteropString_Free;
    public delegate* unmanaged<IntPtr, char*> InteropString_GetData;
    public delegate* unmanaged<IntPtr, char*, void> InteropString_SetData;
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
    public delegate* unmanaged<uint8*, int32, GCHandle> MasterAssemblyLoadContext_LoadAssembly;

    public delegate* unmanaged<GCHandle, IntPtr, uint8> Assembly_GetName;
}

[StructLayout(LayoutKind.Sequential)]
internal struct StartupOutput
{
    public ManagedFunctions ManagedFunctions;
}

internal static class Entry
{
    [UnmanagedCallersOnly]
    private static unsafe int Startup(StartupInput* input, StartupOutput* output)
    {
        // UE
        UE_Interop.GUE_Log = input->UnmanagedFunctions.UE_Log;
        
        // InteropString
        InteropString_Interop.GAlloc = input->UnmanagedFunctions.InteropString_Alloc;
        InteropString_Interop.GFree = input->UnmanagedFunctions.InteropString_Free;
        InteropString_Interop.GGetData = input->UnmanagedFunctions.InteropString_GetData;
        InteropString_Interop.GSetData = input->UnmanagedFunctions.InteropString_SetData;
        
        // CLR interop functions
        output->ManagedFunctions.CLR_CreateMasterALC = &CLR_Interop.CreateMasterALC;
        
        // GCHandle interop functions
        output->ManagedFunctions.GCHandle_Free = &GCHandle_Interop.Free;

        // MasterAssemblyLoadContext interop functions
        output->ManagedFunctions.MasterAssemblyLoadContext_Unload = &MasterAssemblyLoadContext_Interop.Unload;
        output->ManagedFunctions.MasterAssemblyLoadContext_LoadAssembly = &MasterAssemblyLoadContext_Interop.LoadAssembly;
        
        // Assembly interop functions
        output->ManagedFunctions.Assembly_GetName = &Assembly_Interop.GetName;
        
        Logger.Log("====================== CoreCLR Startup ======================");
        
        return 1;
    }
}


