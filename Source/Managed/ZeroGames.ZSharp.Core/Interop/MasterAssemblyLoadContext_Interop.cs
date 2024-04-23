// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;
using System.Reflection;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

internal static class MasterAssemblyLoadContext_Interop
{
    
    [UnmanagedCallersOnly]
    public static int32 Unload()
    {
        MasterAssemblyLoadContext? alc = MasterAssemblyLoadContext.Get();
        if (alc is null)
        {
            return -1;
        }

        alc.Unload();
        return 0;
    }

    [UnmanagedCallersOnly]
    public static unsafe GCHandle LoadAssembly(uint8* buffer, int32 size, void* args)
    {
        MasterAssemblyLoadContext? alc = MasterAssemblyLoadContext.Get();
        if (alc is null)
        {
            return new();
        }
        
        Assembly asm = alc.LoadFromStream(new UnmanagedMemoryStream(buffer, size));
        
        string asmName = asm.FullName!.Split(',')[0];
        Type? entryType = asm.GetType($"{asmName}.__DllEntry");
        if (entryType is not null)
        {
            MethodInfo? dllMain = entryType.GetMethod("DllMain");
            if (dllMain is not null)
            {
                object?[]? parameters = args is not null ? new object?[] { new IntPtr(args) } : null;
                dllMain.Invoke(null, parameters);
            }
        }

        return GCHandle.Alloc(asm, GCHandleType.Weak);
    }
    
    public static unsafe delegate* unmanaged<ZCallHandle, ZCallBuffer*, int32> SZCallByHandle;
    public static unsafe delegate* unmanaged<char*, ZCallBuffer*, ZCallHandle*, uint8, int32> SZCallByName;
    public static unsafe delegate* unmanaged<char*, ZCallHandle> SGetZCallHandle;
    
}