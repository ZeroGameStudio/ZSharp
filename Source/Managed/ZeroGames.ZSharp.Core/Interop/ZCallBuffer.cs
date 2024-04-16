using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

[StructLayout(LayoutKind.Explicit)]
public struct ZCallBufferSlot
{
    [FieldOffset(0)] public uint8 UInt8;
    [FieldOffset(0)] public uint16 UInt16;
    [FieldOffset(0)] public uint32 UInt32;
    [FieldOffset(0)] public uint64 UInt64;
    [FieldOffset(0)] public int8 Int8;
    [FieldOffset(0)] public int16 Int16;
    [FieldOffset(0)] public int32 Int32;
    [FieldOffset(0)] public int64 Int64;
    [FieldOffset(0)] public float Float;
    [FieldOffset(0)] public double Double;
    [FieldOffset(0)] public uint8 Bool;
    [FieldOffset(0)] public ConjugateHandle Conjugate;
}

[StructLayout(LayoutKind.Sequential)]
public struct ZCallBuffer
{
    public unsafe ZCallBufferSlot* Slots;
}


