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

	public static ZCallBufferSlot FromUInt8(uint8 value) => new(EZCallBufferSlotType.UInt8) { _value = new() { UInt8 = value } };
	public static ZCallBufferSlot FromUInt16(uint16 value) => new(EZCallBufferSlotType.UInt16) { _value = new() { UInt16 = value } };
	public static ZCallBufferSlot FromUInt32(uint32 value) => new(EZCallBufferSlotType.UInt32) { _value = new() { UInt32 = value } };
	public static ZCallBufferSlot FromUInt64(uint64 value) => new(EZCallBufferSlotType.UInt64) { _value = new() { UInt64 = value } };
	public static ZCallBufferSlot FromInt8(int8 value) => new(EZCallBufferSlotType.Int8) { _value = new() { Int8 = value } };
	public static ZCallBufferSlot FromInt16(int16 value) => new(EZCallBufferSlotType.Int16) { _value = new() { Int16 = value } };
	public static ZCallBufferSlot FromInt32(int32 value) => new(EZCallBufferSlotType.Int32) { _value = new() { Int32 = value } };
	public static ZCallBufferSlot FromInt64(int64 value) => new(EZCallBufferSlotType.Int64) { _value = new() { Int64 = value } };
	public static ZCallBufferSlot FromFloat(float value) => new(EZCallBufferSlotType.Float) { _value = new() { Float = value } };
	public static ZCallBufferSlot FromDouble(double value) => new(EZCallBufferSlotType.Double) { _value = new() { Double = value } };
	public static ZCallBufferSlot FromBool(bool value) => new(EZCallBufferSlotType.Bool) { _value = new() { Bool = (uint8)(value ? 1 : 0) } };
	public static ZCallBufferSlot FromPointer(IntPtr value) => new(EZCallBufferSlotType.Pointer) { _value = new() { Pointer = value } };
	public static ZCallBufferSlot FromGCHandle(GCHandle value) => new(EZCallBufferSlotType.GCHandle) { _value = new() { GCHandle = value } };
	public static ZCallBufferSlot FromConjugate(ConjugateHandle value) => new(EZCallBufferSlotType.Conjugate) { _value = new() { Conjugate = value } };
	
	public static unsafe ZCallBufferSlot FromPointer(void* value) => FromPointer(new IntPtr(value));
	public static ZCallBufferSlot FromConjugate(IConjugate? value) => FromConjugate(new ConjugateHandle(value));
	
	public uint8 ReadUInt8()
	{
		if (_type != EZCallBufferSlotType.UInt8)
		{
			throw new InvalidOperationException();
		}

		return _value.UInt8;
	}
	
	public uint16 ReadUInt16()
	{
		if (_type != EZCallBufferSlotType.UInt16)
		{
			throw new InvalidOperationException();
		}

		return _value.UInt16;
	}
	
	public uint32 ReadUInt32()
	{
		if (_type != EZCallBufferSlotType.UInt32)
		{
			throw new InvalidOperationException();
		}

		return _value.UInt32;
	}
	
	public uint64 ReadUInt64()
	{
		if (_type != EZCallBufferSlotType.UInt64)
		{
			throw new InvalidOperationException();
		}

		return _value.UInt64;
	}
	
	public int8 ReadInt8()
	{
		if (_type != EZCallBufferSlotType.Int8)
		{
			throw new InvalidOperationException();
		}

		return _value.Int8;
	}
	
	public int16 ReadInt16()
	{
		if (_type != EZCallBufferSlotType.Int16)
		{
			throw new InvalidOperationException();
		}

		return _value.Int16;
	}
	
	public int32 ReadInt32()
	{
		if (_type != EZCallBufferSlotType.Int32)
		{
			throw new InvalidOperationException();
		}

		return _value.Int32;
	}
	
	public int64 ReadInt64()
	{
		if (_type != EZCallBufferSlotType.Int64)
		{
			throw new InvalidOperationException();
		}

		return _value.Int64;
	}
	
	public float ReadFloat()
	{
		if (_type != EZCallBufferSlotType.Float)
		{
			throw new InvalidOperationException();
		}

		return _value.Float;
	}
	
	public double ReadDouble()
	{
		if (_type != EZCallBufferSlotType.Double)
		{
			throw new InvalidOperationException();
		}

		return _value.Double;
	}
	
	public bool ReadBool()
	{
		if (_type != EZCallBufferSlotType.Bool)
		{
			throw new InvalidOperationException();
		}

		return _value.Bool > 0;
	}
	
	public IntPtr ReadPointer()
	{
		if (_type != EZCallBufferSlotType.Pointer)
		{
			throw new InvalidOperationException();
		}

		return _value.Pointer;
	}
	
	public GCHandle ReadGCHandle()
	{
		if (_type != EZCallBufferSlotType.GCHandle)
		{
			throw new InvalidOperationException();
		}

		return _value.GCHandle;
	}
	
	public ConjugateHandle ReadConjugate()
	{
		if (_type != EZCallBufferSlotType.Conjugate)
		{
			throw new InvalidOperationException();
		}

		return _value.Conjugate;
	}
	
	public void WriteUInt8(uint8 value)
	{
		if (_type != EZCallBufferSlotType.UInt8)
		{
			throw new InvalidOperationException();
		}

		_value.UInt8 = value;
	}
	
	public void WriteUInt16(uint16 value)
	{
		if (_type != EZCallBufferSlotType.UInt16)
		{
			throw new InvalidOperationException();
		}

		_value.UInt16 = value;
	}
	
	public void WriteUInt32(uint32 value)
	{
		if (_type != EZCallBufferSlotType.UInt32)
		{
			throw new InvalidOperationException();
		}

		_value.UInt32 = value;
	}
	
	public void WriteUInt64(uint64 value)
	{
		if (_type != EZCallBufferSlotType.UInt64)
		{
			throw new InvalidOperationException();
		}

		_value.UInt64 = value;
	}
	
	public void WriteInt8(int8 value)
	{
		if (_type != EZCallBufferSlotType.Int8)
		{
			throw new InvalidOperationException();
		}

		_value.Int8 = value;
	}
	
	public void WriteInt16(int16 value)
	{
		if (_type != EZCallBufferSlotType.Int16)
		{
			throw new InvalidOperationException();
		}

		_value.Int16 = value;
	}
	
	public void WriteInt32(int32 value)
	{
		if (_type != EZCallBufferSlotType.Int32)
		{
			throw new InvalidOperationException();
		}

		_value.Int32 = value;
	}
	
	public void WriteInt64(int64 value)
	{
		if (_type != EZCallBufferSlotType.Int64)
		{
			throw new InvalidOperationException();
		}

		_value.Int64 = value;
	}
	
	public void WriteFloat(float value)
	{
		if (_type != EZCallBufferSlotType.Float)
		{
			throw new InvalidOperationException();
		}

		_value.Float = value;
	}
	
	public void WriteDouble(double value)
	{
		if (_type != EZCallBufferSlotType.Double)
		{
			throw new InvalidOperationException();
		}

		_value.Double = value;
	}
	
	public void WriteBool(bool value)
	{
		if (_type != EZCallBufferSlotType.Bool)
		{
			throw new InvalidOperationException();
		}

		_value.Bool = (uint8)(value ? 1 : 0);
	}
	
	public void WritePointer(IntPtr value)
	{
		if (_type != EZCallBufferSlotType.Pointer)
		{
			throw new InvalidOperationException();
		}

		_value.Pointer = value;
	}
	
	public void WriteGCHandle(GCHandle value)
	{
		if (_type != EZCallBufferSlotType.GCHandle)
		{
			throw new InvalidOperationException();
		}

		_value.GCHandle = value;
	}
	
	public void WriteConjugate(ConjugateHandle value)
	{
		if (_type != EZCallBufferSlotType.Conjugate)
		{
			throw new InvalidOperationException();
		}

		_value.Conjugate = value;
	}

	public T? ReadConjugate<T>() where T : class, IConjugate => ReadConjugate().GetTarget<T>();
	public void WriteConjugate<T>(T? value) where T : class, IConjugate => WriteConjugate(new ConjugateHandle(value));

	public EZCallBufferSlotType Type => _type;

	public uint8 UInt8
	{
		get => ReadUInt8();
		set => WriteUInt8(value);
	}
	
	public uint16 UInt16
	{
		get => ReadUInt16();
		set => WriteUInt16(value);
	}
	
	public uint32 UInt32
	{
		get => ReadUInt32();
		set => WriteUInt32(value);
	}
	
	public uint64 UInt64
	{
		get => ReadUInt64();
		set => WriteUInt64(value);
	}
	
	public int8 Int8
	{
		get => ReadInt8();
		set => WriteInt8(value);
	}
	
	public int16 Int16
	{
		get => ReadInt16();
		set => WriteInt16(value);
	}
	
	public int32 Int32
	{
		get => ReadInt32();
		set => WriteInt32(value);
	}
	
	public int64 Int64
	{
		get => ReadInt64();
		set => WriteInt64(value);
	}
	
	public float Float
	{
		get => ReadFloat();
		set => WriteDouble(value);
	}
	
	public double Double
	{
		get => ReadDouble();
		set => WriteDouble(value);
	}
	
	public bool Bool
	{
		get => ReadBool();
		set => WriteBool(value);
	}
	
	public IntPtr Pointer
	{
		get => ReadPointer();
		set => WritePointer(value);
	}
	
	public GCHandle GCHandle
	{
		get => ReadGCHandle();
		set => WriteGCHandle(value);
	}
	
	public ConjugateHandle Conjugate
	{
		get => ReadConjugate();
		set => WriteConjugate(value);
	}

	private ZCallBufferSlot(EZCallBufferSlotType type) => _type = type;
	
	private readonly EZCallBufferSlotType _type;
	private ZCallBufferSlotValue _value;
	
}

[StructLayout(LayoutKind.Sequential)]
public unsafe struct ZCallBuffer
{

	public ZCallBuffer(ZCallBufferSlot* slots, int32 numSlots)
	{
		_slots = slots;
		_numSlots = numSlots;
	}

	public ref ZCallBufferSlot this[int32 index]
	{
		get
		{
			if (index < 0)
			{
				index = NumSlots + index;
			}
			
			if (index < 0 || index >= _numSlots)
			{
				throw new ArgumentOutOfRangeException();
			}

			return ref _slots[index];
		}
	}

	public int32 NumSlots => _numSlots;
	
    private ZCallBufferSlot* _slots;
    private int32 _numSlots;
    
}


