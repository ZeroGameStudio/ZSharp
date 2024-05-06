// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

[StructLayout(LayoutKind.Sequential)]
public struct ZCallHandle
{
    
    public bool Valid => _handle > 0;

    private uint64 _handle;

}


