// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

internal static class Assembly_Interop
{

    [UnmanagedCallersOnly]
    public static int32 GetName(GCHandle handle, IntPtr outNameAddr)
    {
        if (handle.Target is Assembly asm)
        {
            InteropString interopStr = new(outNameAddr);
            string fullName = asm.FullName!;
            string name = fullName.Split(',')[0];
            interopStr.Data = name;

            return 0;
        }

        return 1;
    }

    [UnmanagedCallersOnly]
    public static unsafe GCHandle GetType(GCHandle handle, char* name)
    {
        if (handle.Target is Assembly asm)
        {
            Type? type = asm.GetType(new string(name));
            if (type is not null)
            {
                return GCHandle.Alloc(type);
            }
        }

        return new();
    }
    
}


