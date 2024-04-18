using System.Reflection;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

internal static class Assembly_Interop
{

    [UnmanagedCallersOnly]
    public static uint8 GetName(GCHandle handle, IntPtr outNameAddr)
    {
        if (handle.Target is Assembly asm)
        {
            InteropString interopStr = new(outNameAddr);
            string fullName = asm.FullName!;
            string name = fullName.Split(',')[0];
            interopStr.Data = name;

            return 1;
        }

        return 0;
    }

    [UnmanagedCallersOnly]
    public static unsafe GCHandle GetType(GCHandle handle, char* name)
    {
        if (handle.Target is Assembly asm)
        {
            Type? type = asm.GetType(new string(name));
            if (type is not null)
            {
                return GCHandle.Alloc(type, GCHandleType.Weak);
            }
        }

        return new();
    }
    
}


