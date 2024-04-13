using System.Reflection;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp;

internal static class Assembly_Interop
{

    [UnmanagedCallersOnly]
    public static uint8 GetName(GCHandle handle, IntPtr outNameAddr)
    {
        if (handle.Target is InteropProxy<Assembly> proxy)
        {
            Assembly asm = proxy.Inner;
            InteropString interopStr = new(outNameAddr);
            string fullName = asm.FullName!;
            string name = fullName.Split(',')[0];
            interopStr.Data = name;

            return 1;
        }

        return 0;
    }
    
}


