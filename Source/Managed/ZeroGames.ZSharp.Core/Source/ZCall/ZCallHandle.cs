// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

[StructLayout(LayoutKind.Sequential)]
public readonly struct ZCallHandle
{

    public static ZCallHandle Alloc() => new(--_currentHandle);
    
    public bool IsValid => _handle != 0;
    public bool IsRed => _handle < 0;
    public bool IsBlack => _handle > 0;

    private ZCallHandle(int64 handle) => _handle = handle;
    
    private static int64 _currentHandle;

    private readonly int64 _handle;

}


