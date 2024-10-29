// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

[StructLayout(LayoutKind.Sequential)]
public readonly struct ZCallHandle : IEquatable<ZCallHandle>
{

    public static ZCallHandle Alloc() => new(--_currentHandle);

    public override bool Equals(object? obj)
    {
        return obj is ZCallHandle other && Equals(other);
    }

    public override int32 GetHashCode()
    {
        return _handle.GetHashCode();
    }
    
    public bool Equals(ZCallHandle other)
    {
        return _handle == other._handle;
    }
    
    public bool IsValid => _handle != 0;
    public bool IsRed => _handle < 0;
    public bool IsBlack => _handle > 0;

    private ZCallHandle(int64 handle) => _handle = handle;
    
    private static int64 _currentHandle;

    private readonly int64 _handle;
    
}


