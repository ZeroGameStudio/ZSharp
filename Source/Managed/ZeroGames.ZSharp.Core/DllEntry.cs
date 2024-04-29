// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

[StructLayout(LayoutKind.Sequential)]
internal unsafe struct UnmanagedFunctions
{
    public delegate* unmanaged<uint8, char*, void> UnrealEngine_Log;
    public delegate* unmanaged<uint8> UnrealEngine_IsInGameThread;
    
    public delegate* unmanaged<char*, IntPtr> InteropString_Alloc;
    public delegate* unmanaged<IntPtr, void> InteropString_Free;
    public delegate* unmanaged<IntPtr, char*> InteropString_GetData;
    public delegate* unmanaged<IntPtr, char*, void> InteropString_SetData;

    public delegate* unmanaged<ZCallHandle, ZCallBuffer*, int32> MasterAssemblyLoadContext_ZCallByHandle;
    public delegate* unmanaged<char*, ZCallBuffer*, ZCallHandle*, int32> MasterAssemblyLoadContext_ZCallByName;
    public delegate* unmanaged<ZCallHandle, ZCallBuffer*, int32, void> MasterAssemblyLoadContext_ZCallByHandle_AnyThread;
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
    public delegate* unmanaged<int32, uint8, uint8, uint8, void> CLR_CollectGarbage;
    public delegate* unmanaged<GCHandle> CLR_CreateMasterALC;
    public delegate* unmanaged<char*, GCHandle> CLR_CreateSlimALC;
    
    public delegate* unmanaged<GCHandle, int32> GCHandle_Free;

    public delegate* unmanaged<int32> MasterAssemblyLoadContext_Unload;
    public delegate* unmanaged<uint8*, int32, void*, GCHandle> MasterAssemblyLoadContext_LoadAssembly;
    public delegate* unmanaged<ConjugateHandle, int32> MasterAssemblyLoadContext_ReleaseConjugate;

    public delegate* unmanaged<GCHandle, int32> SlimAssemblyLoadContext_Unload;
    public delegate* unmanaged<GCHandle, uint8*, int32, void*, int32> SlimAssemblyLoadContext_LoadAssembly;
    public delegate* unmanaged<GCHandle, char*, char*, char*, void*, int32> SlimAssemblyLoadContext_CallMethod;
    
    public delegate* unmanaged<GCHandle, IntPtr, int32> Assembly_GetName;
    public delegate* unmanaged<GCHandle, char*, GCHandle> Assembly_GetType;
    
    public delegate* unmanaged<GCHandle, IntPtr, int32> Type_GetName;
    public delegate* unmanaged<GCHandle, IntPtr, ConjugateHandle> Type_BuildConjugate;
    public delegate* unmanaged<GCHandle, char*, GCHandle> Type_GetMethodInfo;
    public delegate* unmanaged<GCHandle, char*, GCHandle> Type_GetPropertyInfo;
    
    public delegate* unmanaged<GCHandle, IntPtr, int32> MethodInfo_GetName;
    public delegate* unmanaged<GCHandle, int32> MethodInfo_GetNumSlots;
    public delegate* unmanaged<GCHandle, ZCallBuffer*, int32> MethodInfo_Invoke;
}

[StructLayout(LayoutKind.Sequential)]
internal struct StartupOutput
{
    public ManagedFunctions ManagedFunctions;
}

internal static class DllEntry
{
    [UnmanagedCallersOnly]
    private static unsafe int32 DllMain(StartupInput* input, StartupOutput* output)
    {
        // UE
        UnrealEngine_Interop.SLog = input->UnmanagedFunctions.UnrealEngine_Log;
        UnrealEngine_Interop.SIsInGameThread = input->UnmanagedFunctions.UnrealEngine_IsInGameThread;
        
        // InteropString
        InteropString_Interop.SAlloc = input->UnmanagedFunctions.InteropString_Alloc;
        InteropString_Interop.SFree = input->UnmanagedFunctions.InteropString_Free;
        InteropString_Interop.SGetData = input->UnmanagedFunctions.InteropString_GetData;
        InteropString_Interop.SSetData = input->UnmanagedFunctions.InteropString_SetData;
        
        // MasterAssemblyLoadContext
        MasterAssemblyLoadContext_Interop.SZCallByHandle = input->UnmanagedFunctions.MasterAssemblyLoadContext_ZCallByHandle;
        MasterAssemblyLoadContext_Interop.SZCallByName = input->UnmanagedFunctions.MasterAssemblyLoadContext_ZCallByName;
        MasterAssemblyLoadContext_Interop.SZCallByHandle_AnyThread = input->UnmanagedFunctions.MasterAssemblyLoadContext_ZCallByHandle_AnyThread;
        MasterAssemblyLoadContext_Interop.SGetZCallHandle = input->UnmanagedFunctions.MasterAssemblyLoadContext_GetZCallHandle;
        
        // CLR interop functions
        output->ManagedFunctions.CLR_CollectGarbage = &CLR_Interop.CollectGarbage;
        output->ManagedFunctions.CLR_CreateMasterALC = &CLR_Interop.CreateMasterALC;
        output->ManagedFunctions.CLR_CreateSlimALC = &CLR_Interop.CreateSlimALC;
        
        // GCHandle interop functions
        output->ManagedFunctions.GCHandle_Free = &GCHandle_Interop.Free;

        // MasterAssemblyLoadContext interop functions
        output->ManagedFunctions.MasterAssemblyLoadContext_Unload = &MasterAssemblyLoadContext_Interop.Unload;
        output->ManagedFunctions.MasterAssemblyLoadContext_LoadAssembly = &MasterAssemblyLoadContext_Interop.LoadAssembly;
        output->ManagedFunctions.MasterAssemblyLoadContext_ReleaseConjugate = &MasterAssemblyLoadContext_Interop.ReleaseConjugate;

        // SlimAssemblyLoadContext interop functions
        output->ManagedFunctions.SlimAssemblyLoadContext_Unload = &SlimAssemblyLoadContext_Interop.Unload;
        output->ManagedFunctions.SlimAssemblyLoadContext_LoadAssembly = &SlimAssemblyLoadContext_Interop.LoadAssembly;
        output->ManagedFunctions.SlimAssemblyLoadContext_CallMethod = &SlimAssemblyLoadContext_Interop.CallMethod;
        
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
        output->ManagedFunctions.MethodInfo_GetNumSlots = &MethodInfo_Interop.GetNumSlots;
        output->ManagedFunctions.MethodInfo_Invoke = &MethodInfo_Interop.Invoke;
        
        Logger.Log("====================== ZSharp Startup ======================");

        return 1;
    }
}


