using System.Reflection;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp;

internal static unsafe class MasterAssemblyLoadContext_Interop
{
    
    [UnmanagedCallersOnly]
    public static void Unload()
    {
        MasterAssemblyLoadContext.UnloadSingleton();
    }

    [UnmanagedCallersOnly]
    public static GCHandle LoadAssembly(uint8* buffer, int32 size)
    {
        MasterAssemblyLoadContext alc = MasterAssemblyLoadContext.Get()!;
        
        uint8[] bufferArr = new uint8[size];
        for (int32 offset = 0; offset < size; ++offset)
        {
            bufferArr[offset] = *(buffer + offset);
        }

        Assembly asm = alc.LoadFromStream(new MemoryStream(bufferArr));
        InteropProxy<Assembly> proxy = new(asm);

        return proxy.GCHandle;
    }
    
}