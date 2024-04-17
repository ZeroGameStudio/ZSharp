using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

[StructLayout(LayoutKind.Sequential)]
public struct ZCallHandle
{
    
    public bool bValid => _handle > 0;

    private uint64 _handle;

}


