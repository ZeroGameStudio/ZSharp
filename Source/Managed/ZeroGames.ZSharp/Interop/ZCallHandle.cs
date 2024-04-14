using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp;

[StructLayout(LayoutKind.Sequential)]
public struct ZCallHandle
{
    
    public bool IsValid => _handle > 0;

    private uint64 _handle;

}


