// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

public enum EZCallBufferSlotType : uint8
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
	Pointer,
	GCHandle,
	Conjugate,
}

[StructLayout(LayoutKind.Explicit)]
public struct ZCallBufferSlotValue
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
    [FieldOffset(0)] public IntPtr Pointer;
    [FieldOffset(0)] public GCHandle GCHandle;
    [FieldOffset(0)] public ConjugateHandle Conjugate;
}

[StructLayout(LayoutKind.Sequential)]
public struct ZCallBufferSlot
{

	public static ZCallBufferSlot FromUInt8(uint8 value) => new(EZCallBufferSlotType.UInt8) { Value = new() { UInt8 = value } };
	public static ZCallBufferSlot FromUInt16(uint16 value) => new(EZCallBufferSlotType.UInt16) { Value = new() { UInt16 = value } };
	public static ZCallBufferSlot FromUInt32(uint32 value) => new(EZCallBufferSlotType.UInt32) { Value = new() { UInt32 = value } };
	public static ZCallBufferSlot FromUInt64(uint64 value) => new(EZCallBufferSlotType.UInt64) { Value = new() { UInt64 = value } };
	public static ZCallBufferSlot FromInt8(int8 value) => new(EZCallBufferSlotType.Int8) { Value = new() { Int8 = value } };
	public static ZCallBufferSlot FromInt16(int16 value) => new(EZCallBufferSlotType.Int16) { Value = new() { Int16 = value } };
	public static ZCallBufferSlot FromInt32(int32 value) => new(EZCallBufferSlotType.Int32) { Value = new() { Int32 = value } };
	public static ZCallBufferSlot FromInt64(int64 value) => new(EZCallBufferSlotType.Int64) { Value = new() { Int64 = value } };
	public static ZCallBufferSlot FromFloat(float value) => new(EZCallBufferSlotType.Float) { Value = new() { Float = value } };
	public static ZCallBufferSlot FromDouble(double value) => new(EZCallBufferSlotType.Double) { Value = new() { Double = value } };
	public static ZCallBufferSlot FromBool(bool value) => new(EZCallBufferSlotType.Bool) { Value = new() { Bool = (uint8)(value ? 1 : 0) } };
	public static ZCallBufferSlot FromPointer(IntPtr value) => new(EZCallBufferSlotType.Pointer) { Value = new() { Pointer = value } };
	public static ZCallBufferSlot FromGCHandle(GCHandle value) => new(EZCallBufferSlotType.GCHandle) { Value = new() { GCHandle = value } };
	public static ZCallBufferSlot FromConjugate(ConjugateHandle value) => new(EZCallBufferSlotType.Conjugate) { Value = new() { Conjugate = value } };
	
	public static unsafe ZCallBufferSlot FromPointer(void* value) => FromPointer(new IntPtr(value));
	public static ZCallBufferSlot FromConjugate(IConjugate? value) => FromConjugate(value.GetConjugateHandle());
	
	public uint8 ReadUInt8()
	{
		if (Type != EZCallBufferSlotType.UInt8)
		{
			throw new InvalidOperationException();
		}

		return Value.UInt8;
	}
	
	public uint16 ReadUInt16()
	{
		if (Type != EZCallBufferSlotType.UInt16)
		{
			throw new InvalidOperationException();
		}

		return Value.UInt16;
	}
	
	public uint32 ReadUInt32()
	{
		if (Type != EZCallBufferSlotType.UInt32)
		{
			throw new InvalidOperationException();
		}

		return Value.UInt32;
	}
	
	public uint64 ReadUInt64()
	{
		if (Type != EZCallBufferSlotType.UInt64)
		{
			throw new InvalidOperationException();
		}

		return Value.UInt64;
	}
	
	public int8 ReadInt8()
	{
		if (Type != EZCallBufferSlotType.Int8)
		{
			throw new InvalidOperationException();
		}

		return Value.Int8;
	}
	
	public int16 ReadInt16()
	{
		if (Type != EZCallBufferSlotType.Int16)
		{
			throw new InvalidOperationException();
		}

		return Value.Int16;
	}
	
	public int32 ReadInt32()
	{
		if (Type != EZCallBufferSlotType.Int32)
		{
			throw new InvalidOperationException();
		}

		return Value.Int32;
	}
	
	public int64 ReadInt64()
	{
		if (Type != EZCallBufferSlotType.Int64)
		{
			throw new InvalidOperationException();
		}

		return Value.Int64;
	}
	
	public float ReadFloat()
	{
		if (Type != EZCallBufferSlotType.Float)
		{
			throw new InvalidOperationException();
		}

		return Value.Float;
	}
	
	public double ReadDouble()
	{
		if (Type != EZCallBufferSlotType.Double)
		{
			throw new InvalidOperationException();
		}

		return Value.Double;
	}
	
	public bool ReadBool()
	{
		if (Type != EZCallBufferSlotType.Bool)
		{
			throw new InvalidOperationException();
		}

		return Value.Bool > 0;
	}
	
	public IntPtr ReadPointer()
	{
		if (Type != EZCallBufferSlotType.Pointer)
		{
			throw new InvalidOperationException();
		}

		return Value.Pointer;
	}
	
	public GCHandle ReadGCHandle()
	{
		if (Type != EZCallBufferSlotType.GCHandle)
		{
			throw new InvalidOperationException();
		}

		return Value.GCHandle;
	}
	
	public ConjugateHandle ReadConjugate()
	{
		if (Type != EZCallBufferSlotType.Conjugate)
		{
			throw new InvalidOperationException();
		}

		return Value.Conjugate;
	}
	
	public void WriteUInt8(uint8 value)
	{
		if (Type != EZCallBufferSlotType.UInt8)
		{
			throw new InvalidOperationException();
		}

		Value.UInt8 = value;
	}
	
	public void WriteUInt16(uint16 value)
	{
		if (Type != EZCallBufferSlotType.UInt16)
		{
			throw new InvalidOperationException();
		}

		Value.UInt16 = value;
	}
	
	public void WriteUInt32(uint32 value)
	{
		if (Type != EZCallBufferSlotType.UInt32)
		{
			throw new InvalidOperationException();
		}

		Value.UInt32 = value;
	}
	
	public void WriteUInt64(uint64 value)
	{
		if (Type != EZCallBufferSlotType.UInt64)
		{
			throw new InvalidOperationException();
		}

		Value.UInt64 = value;
	}
	
	public void WriteInt8(int8 value)
	{
		if (Type != EZCallBufferSlotType.Int8)
		{
			throw new InvalidOperationException();
		}

		Value.Int8 = value;
	}
	
	public void WriteInt16(int16 value)
	{
		if (Type != EZCallBufferSlotType.Int16)
		{
			throw new InvalidOperationException();
		}

		Value.Int16 = value;
	}
	
	public void WriteInt32(int32 value)
	{
		if (Type != EZCallBufferSlotType.Int32)
		{
			throw new InvalidOperationException();
		}

		Value.Int32 = value;
	}
	
	public void WriteInt64(int64 value)
	{
		if (Type != EZCallBufferSlotType.Int64)
		{
			throw new InvalidOperationException();
		}

		Value.Int64 = value;
	}
	
	public void WriteFloat(float value)
	{
		if (Type != EZCallBufferSlotType.Float)
		{
			throw new InvalidOperationException();
		}

		Value.Float = value;
	}
	
	public void WriteDouble(double value)
	{
		if (Type != EZCallBufferSlotType.Double)
		{
			throw new InvalidOperationException();
		}

		Value.Double = value;
	}
	
	public void WriteBool(bool value)
	{
		if (Type != EZCallBufferSlotType.Bool)
		{
			throw new InvalidOperationException();
		}

		Value.Bool = (uint8)(value ? 1 : 0);
	}
	
	public void WritePointer(IntPtr value)
	{
		if (Type != EZCallBufferSlotType.Pointer)
		{
			throw new InvalidOperationException();
		}

		Value.Pointer = value;
	}
	
	public void WriteGCHandle(GCHandle value)
	{
		if (Type != EZCallBufferSlotType.GCHandle)
		{
			throw new InvalidOperationException();
		}

		Value.GCHandle = value;
	}
	
	public void WriteConjugate(ConjugateHandle value)
	{
		if (Type != EZCallBufferSlotType.Conjugate)
		{
			throw new InvalidOperationException();
		}

		Value.Conjugate = value;
	}

	public T? ReadConjugate<T>() where T : class, IConjugate => ReadConjugate().ToConjugate<T>();
	public void WriteConjugate<T>(T? value) where T : class, IConjugate => WriteConjugate(value.GetConjugateHandle());

	private ZCallBufferSlot(EZCallBufferSlotType type) => Type = type;
	
	private readonly EZCallBufferSlotType Type;
	private ZCallBufferSlotValue Value;
	
}

[StructLayout(LayoutKind.Sequential)]
public unsafe struct ZCallBuffer
{

	public ZCallBuffer(ZCallBufferSlot* slots, int32 numSlots)
	{
		Slots = slots;
		NumSlots = numSlots;
	}

	public ref ZCallBufferSlot this[int32 index]
	{
		get
		{
			if (index < 0 || index >= NumSlots)
			{
				throw new ArgumentOutOfRangeException();
			}

			return ref Slots[index];
		}
	}
	
    private ZCallBufferSlot* Slots;
    private int32 NumSlots;
    
}


