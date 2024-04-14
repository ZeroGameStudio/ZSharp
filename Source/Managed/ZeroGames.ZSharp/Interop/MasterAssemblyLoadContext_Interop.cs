using System.Reflection;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp;

internal static class MasterAssemblyLoadContext_Interop
{
    
    [UnmanagedCallersOnly]
    public static void Unload()
    {
        MasterAssemblyLoadContext.UnloadSingleton();
    }

    [UnmanagedCallersOnly]
    public static unsafe GCHandle LoadAssembly(uint8* buffer, int32 size)
    {
        MasterAssemblyLoadContext alc = MasterAssemblyLoadContext.Get()!;
        Assembly asm = alc.LoadFromStream(new UnmanagedMemoryStream(buffer, size));
        InteropProxy<Assembly> proxy = new(asm);

        string asmName = asm.FullName!.Split(',')[0];
        Type? entryType = asm.GetType($"{asmName}.Entry");
        if (entryType is not null)
        {
            MethodInfo? startupMethod = entryType.GetMethod("Startup");
            if (startupMethod is not null)
            {
                startupMethod.Invoke(null, null);
            }
        }

        return proxy.GCHandle;
    }
    
    public static unsafe delegate* unmanaged<ZCallHandle, ZCallBuffer*, int32> GZCallByHandle;
    public static unsafe delegate* unmanaged<char*, ZCallBuffer*, ZCallHandle*, int32> GZCallByName;
    public static unsafe delegate* unmanaged<char*, ZCallHandle> GGetZCallHandle;
    
}