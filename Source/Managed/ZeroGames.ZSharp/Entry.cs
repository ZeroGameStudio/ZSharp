using System.Reflection;
using System.Runtime.InteropServices;
using ZeroGames.ZSharp.Interop;

namespace ZeroGames.ZSharp;

[StructLayout(LayoutKind.Sequential)]
internal unsafe struct UnmanagedFunctions
{
    public delegate* unmanaged<uint8, char*, void> UE_Log;
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
}

[StructLayout(LayoutKind.Sequential)]
internal struct ManagedObjects
{
    public GCHandle TypeofAssemblyProxy;
    public GCHandle TypeofTypeProxy;
    public GCHandle TypeofMethodInfoProxy;
    public GCHandle TypeofPropertyInfoProxy;
    public GCHandle TypeofAttributeProxy;
}

[StructLayout(LayoutKind.Sequential)]
internal struct StartupOutput
{
    public ManagedFunctions ManagedFunctions;
    public ManagedObjects ManagedObjects;
}

internal static class Entry
{
    [UnmanagedCallersOnly]
    private static unsafe int Startup(StartupInput* input, StartupOutput* output)
    {
        // UE
        UE_Interop.GUE_Log = input->UnmanagedFunctions.UE_Log;
        
        // CLR interop functions
        output->ManagedFunctions.CLR_CreateMasterALC = &CLR_Interop.CreateMasterALC;
        
        // GCHandle interop functions
        output->ManagedFunctions.GCHandle_Free = &GCHandle_Interop.Free;
        
        // Interop objects
        output->ManagedObjects.TypeofAssemblyProxy = GCHandle.Alloc(typeof(InteropProxy<Assembly>), GCHandleType.Weak);
        output->ManagedObjects.TypeofTypeProxy = GCHandle.Alloc(typeof(InteropProxy<Type>), GCHandleType.Weak);
        output->ManagedObjects.TypeofMethodInfoProxy = GCHandle.Alloc(typeof(InteropProxy<MethodInfo>), GCHandleType.Weak);
        output->ManagedObjects.TypeofPropertyInfoProxy = GCHandle.Alloc(typeof(InteropProxy<PropertyInfo>), GCHandleType.Weak);
        output->ManagedObjects.TypeofAttributeProxy = GCHandle.Alloc(typeof(InteropProxy<Attribute>), GCHandleType.Weak);
        
        Logger.Error("======================== CoreCLR Startup ======================");
        
        return 1;
    }
}


