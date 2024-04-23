using System.Reflection;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

internal static class MasterAssemblyLoadContext_Interop
{
    
    [UnmanagedCallersOnly]
    public static void Unload()
    {
        MasterAssemblyLoadContext.UnloadSingleton();
    }

    [UnmanagedCallersOnly]
    public static unsafe GCHandle LoadAssembly(uint8* buffer, int32 size, void* args)
    {
        MasterAssemblyLoadContext alc = MasterAssemblyLoadContext.Get()!;
        Assembly asm = alc.LoadFromStream(new UnmanagedMemoryStream(buffer, size));
        
        string asmName = asm.FullName!.Split(',')[0];
        Type? entryType = asm.GetType($"{asmName}.Entry");
        if (entryType is not null)
        {
            MethodInfo? startupMethod = entryType.GetMethod("DllMain");
            if (startupMethod is not null)
            {
                object?[]? parameters = args is not null ? new object?[] { new IntPtr(args) } : null;
                startupMethod.Invoke(null, parameters);
            }
        }

        return GCHandle.Alloc(asm, GCHandleType.Weak);
    }
    
    public static unsafe delegate* unmanaged<ZCallHandle, ZCallBuffer*, int32> GZCallByHandle;
    public static unsafe delegate* unmanaged<char*, ZCallBuffer*, ZCallHandle*, uint8, int32> GZCallByName;
    public static unsafe delegate* unmanaged<char*, ZCallHandle> GGetZCallHandle;
    
}