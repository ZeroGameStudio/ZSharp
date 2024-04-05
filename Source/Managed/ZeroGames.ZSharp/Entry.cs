using System.Buffers;
using System.Diagnostics;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp;

public class Entry
{
    [UnmanagedCallersOnly]
    unsafe public static int Startup()
    {
        return 1;
    }
}
