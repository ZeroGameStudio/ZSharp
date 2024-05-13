// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

[StructLayout(LayoutKind.Sequential)]
public readonly struct ZCallHandle
{

    public static ZCallHandle Alloc() => new(--_sCurrentHandle);
    
    public bool Valid => _handle != 0;
    public bool Red => _handle < 0;
    public bool Black => _handle > 0;

    private ZCallHandle(int64 handle) => _handle = handle;
    
    private static int64 _sCurrentHandle;

    private readonly int64 _handle;

}


