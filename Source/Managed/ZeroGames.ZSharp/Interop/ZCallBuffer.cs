using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp;

public enum ZCallBufferSlotType : uint8
{
    UInt8,
    UInt16,
    UInt32,
    UInt64,
    Int8,
    Int16,
    Int32,
    Int64,
    Float,
    Double,
    Bool,
    Conjugate,
}

[StructLayout(LayoutKind.Sequential)]
public struct ZCallBufferSlot
{
    public ZCallBufferSlotType Type;
    public ZCallBufferSlotContent Content;

    [StructLayout(LayoutKind.Explicit)]
    public struct ZCallBufferSlotContent
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
        [FieldOffset(0)] public GCHandle Conjugate;
    }
}

[StructLayout(LayoutKind.Sequential)]
public struct ZCallBuffer
{
    public unsafe ZCallBufferSlot* Slots;
    public uint8 SlotNum;
}


