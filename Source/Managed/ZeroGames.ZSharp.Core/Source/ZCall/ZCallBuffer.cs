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

	public static ZCallBufferSlot FromUInt8(uint8 value = default) => new(EZCallBufferSlotType.UInt8) { _value = new() { UInt8 = value } };
	public static ZCallBufferSlot FromUInt16(uint16 value = default) => new(EZCallBufferSlotType.UInt16) { _value = new() { UInt16 = value } };
	public static ZCallBufferSlot FromUInt32(uint32 value = default) => new(EZCallBufferSlotType.UInt32) { _value = new() { UInt32 = value } };
	public static ZCallBufferSlot FromUInt64(uint64 value = default) => new(EZCallBufferSlotType.UInt64) { _value = new() { UInt64 = value } };
	public static ZCallBufferSlot FromInt8(int8 value = default) => new(EZCallBufferSlotType.Int8) { _value = new() { Int8 = value } };
	public static ZCallBufferSlot FromInt16(int16 value = default) => new(EZCallBufferSlotType.Int16) { _value = new() { Int16 = value } };
	public static ZCallBufferSlot FromInt32(int32 value = default) => new(EZCallBufferSlotType.Int32) { _value = new() { Int32 = value } };
	public static ZCallBufferSlot FromInt64(int64 value = default) => new(EZCallBufferSlotType.Int64) { _value = new() { Int64 = value } };
	public static ZCallBufferSlot FromFloat(float value = default) => new(EZCallBufferSlotType.Float) { _value = new() { Float = value } };
	public static ZCallBufferSlot FromDouble(double value = default) => new(EZCallBufferSlotType.Double) { _value = new() { Double = value } };
	public static ZCallBufferSlot FromBool(bool value = default) => new(EZCallBufferSlotType.Bool) { _value = new() { Bool = (uint8)(value ? 1 : 0) } };
	public static ZCallBufferSlot FromPointer(IntPtr value = default) => new(EZCallBufferSlotType.Pointer) { _value = new() { Pointer = value } };
	public static ZCallBufferSlot FromGCHandle(GCHandle value = default) => new(EZCallBufferSlotType.GCHandle) { _value = new() { GCHandle = value } };
	public static ZCallBufferSlot FromConjugate(ConjugateHandle value) => new(EZCallBufferSlotType.Conjugate) { _value = new() { Conjugate = value } };
	
	public static unsafe ZCallBufferSlot FromPointer(void* value) => FromPointer(new IntPtr(value));
	public static ZCallBufferSlot FromConjugate(IConjugate? value = default) => FromConjugate(ConjugateHandle.FromConjugate(value));

	public static ZCallBufferSlot FromType(Type t)
	{
		if (t.IsAssignableTo(typeof(IConjugate)))
		{
			return FromConjugate();
		}
		else if (t == typeof(uint8))
		{
			return FromUInt8();
		}
		else if (t == typeof(uint16))
		{
			return FromUInt16();
		}
		else if (t == typeof(uint32))
		{
			return FromUInt32();
		}
		else if (t == typeof(uint64))
		{
			return FromUInt64();
		}
		else if (t == typeof(int8))
		{
			return FromInt8();
		}
		else if (t == typeof(int16))
		{
			return FromInt16();
		}
		else if (t == typeof(int32))
		{
			return FromInt32();
		}
		else if (t == typeof(int64))
		{
			return FromInt64();
		}
		else if (t == typeof(float))
		{
			return FromFloat();
		}
		else if (t == typeof(double))
		{
			return FromDouble();
		}
		else if (t == typeof(bool))
		{
			return FromBool();
		}
		else if (t == typeof(IntPtr))
		{
			return FromPointer();
		}
		else if (t == typeof(GCHandle))
		{
			return FromGCHandle();
		}
		else if (t == typeof(ConjugateHandle))
		{
			return FromConjugate();
		}
		else if (t.IsEnum)
		{
			return FromType(t.GetEnumUnderlyingType());
		}
		else
		{
			throw Thrower.NoEntry();
		}
	}

	public static ZCallBufferSlot FromObject(object? value = default)
	{
		if (value is null)
		{
			return FromConjugate();
		}
		
		Type t = value.GetType();
		if (t == typeof(ZCallBufferSlot))
		{
			return (ZCallBufferSlot)value;
		}
		if (t.IsAssignableTo(typeof(Type)))
		{
			return FromType((Type)value);
		}
		if (t.IsAssignableTo(typeof(IConjugate)))
		{
			return FromConjugate((IConjugate)value);
		}
		else if (t == typeof(uint8))
		{
			return FromUInt8((uint8)value);
		}
		else if (t == typeof(uint16))
		{
			return FromUInt16((uint16)value);
		}
		else if (t == typeof(uint32))
		{
			return FromUInt32((uint32)value);
		}
		else if (t == typeof(uint64))
		{
			return FromUInt64((uint64)value);
		}
		else if (t == typeof(int8))
		{
			return FromInt8((int8)value);
		}
		else if (t == typeof(int16))
		{
			return FromInt16((int16)value);
		}
		else if (t == typeof(int32))
		{
			return FromInt32((int32)value);
		}
		else if (t == typeof(int64))
		{
			return FromInt64((int64)value);
		}
		else if (t == typeof(float))
		{
			return FromFloat((float)value);
		}
		else if (t == typeof(double))
		{
			return FromDouble((double)value);
		}
		else if (t == typeof(bool))
		{
			return FromBool((bool)value);
		}
		else if (t == typeof(IntPtr))
		{
			return FromPointer((IntPtr)value);
		}
		else if (t == typeof(GCHandle))
		{
			return FromGCHandle((GCHandle)value);
		}
		else if (t == typeof(ConjugateHandle))
		{
			return FromConjugate((ConjugateHandle)value);
		}
		else if (t.IsEnum)
		{
			// @FIXME
			return FromObject(Convert.ChangeType(value, t.GetEnumUnderlyingType()));
		}
		else
		{
			throw Thrower.NoEntry();
		}
	}
	
	public readonly uint8 ReadUInt8()
	{
		Thrower.FatalIf(_type != EZCallBufferSlotType.UInt8);
		return _value.UInt8;
	}
	
	public readonly uint16 ReadUInt16()
	{
		Thrower.FatalIf(_type != EZCallBufferSlotType.UInt16);
		return _value.UInt16;
	}
	
	public readonly uint32 ReadUInt32()
	{
		Thrower.FatalIf(_type != EZCallBufferSlotType.UInt32);
		return _value.UInt32;
	}
	
	public readonly uint64 ReadUInt64()
	{
		Thrower.FatalIf(_type != EZCallBufferSlotType.UInt64);
		return _value.UInt64;
	}
	
	public readonly int8 ReadInt8()
	{
		Thrower.FatalIf(_type != EZCallBufferSlotType.Int8);
		return _value.Int8;
	}
	
	public readonly int16 ReadInt16()
	{
		Thrower.FatalIf(_type != EZCallBufferSlotType.Int16);
		return _value.Int16;
	}
	
	public readonly int32 ReadInt32()
	{
		Thrower.FatalIf(_type != EZCallBufferSlotType.Int32);
		return _value.Int32;
	}
	
	public readonly int64 ReadInt64()
	{
		Thrower.FatalIf(_type != EZCallBufferSlotType.Int64);
		return _value.Int64;
	}
	
	public readonly float ReadFloat()
	{
		Thrower.FatalIf(_type != EZCallBufferSlotType.Float);
		return _value.Float;
	}
	
	public readonly double ReadDouble()
	{
		Thrower.FatalIf(_type != EZCallBufferSlotType.Double);
		return _value.Double;
	}
	
	public readonly bool ReadBool()
	{
		Thrower.FatalIf(_type != EZCallBufferSlotType.Bool);
		return _value.Bool > 0;
	}
	
	public readonly IntPtr ReadPointer()
	{
		Thrower.FatalIf(_type != EZCallBufferSlotType.Pointer);
		return _value.Pointer;
	}
	
	public readonly GCHandle ReadGCHandle()
	{
		Thrower.FatalIf(_type != EZCallBufferSlotType.GCHandle);
		return _value.GCHandle;
	}
	
	public readonly ConjugateHandle ReadConjugate()
	{
		Thrower.FatalIf(_type != EZCallBufferSlotType.Conjugate);
		return _value.Conjugate;
	}
	
	public void WriteUInt8(uint8 value)
	{
		Thrower.FatalIf(_type != EZCallBufferSlotType.UInt8);
		_value.UInt8 = value;
	}
	
	public void WriteUInt16(uint16 value)
	{
		Thrower.FatalIf(_type != EZCallBufferSlotType.UInt16);
		_value.UInt16 = value;
	}
	
	public void WriteUInt32(uint32 value)
	{
		Thrower.FatalIf(_type != EZCallBufferSlotType.UInt32);
		_value.UInt32 = value;
	}
	
	public void WriteUInt64(uint64 value)
	{
		Thrower.FatalIf(_type != EZCallBufferSlotType.UInt64);
		_value.UInt64 = value;
	}
	
	public void WriteInt8(int8 value)
	{
		Thrower.FatalIf(_type != EZCallBufferSlotType.Int8);
		_value.Int8 = value;
	}
	
	public void WriteInt16(int16 value)
	{
		Thrower.FatalIf(_type != EZCallBufferSlotType.Int16);
		_value.Int16 = value;
	}
	
	public void WriteInt32(int32 value)
	{
		Thrower.FatalIf(_type != EZCallBufferSlotType.Int32);
		_value.Int32 = value;
	}
	
	public void WriteInt64(int64 value)
	{
		Thrower.FatalIf(_type != EZCallBufferSlotType.Int64);
		_value.Int64 = value;
	}
	
	public void WriteFloat(float value)
	{
		Thrower.FatalIf(_type != EZCallBufferSlotType.Float);
		_value.Float = value;
	}
	
	public void WriteDouble(double value)
	{
		Thrower.FatalIf(_type != EZCallBufferSlotType.Double);
		_value.Double = value;
	}
	
	public void WriteBool(bool value)
	{
		Thrower.FatalIf(_type != EZCallBufferSlotType.Bool);
		_value.Bool = (uint8)(value ? 1 : 0);
	}
	
	public void WritePointer(IntPtr value)
	{
		Thrower.FatalIf(_type != EZCallBufferSlotType.Pointer);
		_value.Pointer = value;
	}
	
	public void WriteGCHandle(GCHandle value)
	{
		Thrower.FatalIf(_type != EZCallBufferSlotType.GCHandle);
		_value.GCHandle = value;
	}
	
	public void WriteConjugate(ConjugateHandle value)
	{
		Thrower.FatalIf(_type != EZCallBufferSlotType.Conjugate);
		_value.Conjugate = value;
	}

	public readonly T? ReadConjugate<T>() where T : class, IConjugate => ReadConjugate().GetTarget<T>();
	public readonly T ReadConjugateChecked<T>() where T : class, IConjugate => ReadConjugate().GetTargetChecked<T>();
	public void WriteConjugate<T>(T? value) where T : class, IConjugate => WriteConjugate(ConjugateHandle.FromConjugate(value));

	public readonly object? ReadObject()
	{
		return _type switch
		{
			EZCallBufferSlotType.UInt8 => UInt8,
			EZCallBufferSlotType.UInt16 => UInt16,
			EZCallBufferSlotType.UInt32 => UInt32,
			EZCallBufferSlotType.UInt64 => UInt64,
			EZCallBufferSlotType.Int8 => Int8,
			EZCallBufferSlotType.Int16 => Int16,
			EZCallBufferSlotType.Int32 => Int32,
			EZCallBufferSlotType.Int64 => Int64,
			EZCallBufferSlotType.Float => Float,
			EZCallBufferSlotType.Double => Double,
			EZCallBufferSlotType.Bool => Bool,
			EZCallBufferSlotType.Pointer => Pointer,
			EZCallBufferSlotType.GCHandle => GCHandle.Target,
			EZCallBufferSlotType.Conjugate => Conjugate.GetTarget<IConjugate>(),
			_ => throw Thrower.NoEntry()
		};
	}

	public void WriteObject(object? value)
	{
		if (value is null)
		{
			Conjugate = default;
			return;
		}
		
		Type t = value.GetType();
		if (t.IsAssignableTo(typeof(IConjugate)))
		{
			Conjugate = ConjugateHandle.FromConjugate((IConjugate)value);
		}
		else if (t == typeof(uint8))
		{
			UInt8 = (uint8)value;
		}
		else if (t == typeof(uint16))
		{
			UInt16 = (uint16)value;
		}
		else if (t == typeof(uint32))
		{
			UInt32 = (uint32)value;
		}
		else if (t == typeof(uint64))
		{
			UInt64 = (uint64)value;
		}
		else if (t == typeof(int8))
		{
			Int8 = (int8)value;
		}
		else if (t == typeof(int16))
		{
			Int16 = (int16)value;
		}
		else if (t == typeof(int32))
		{
			Int32 = (int32)value;
		}
		else if (t == typeof(int64))
		{
			Int64 = (int64)value;
		}
		else if (t == typeof(float))
		{
			Float = (float)value;
		}
		else if (t == typeof(double))
		{
			Double = (double)value;
		}
		else if (t == typeof(bool))
		{
			Bool = (bool)value;
		}
		else if (t == typeof(IntPtr))
		{
			Pointer = (IntPtr)value;
		}
		else if (t == typeof(GCHandle))
		{
			GCHandle = (GCHandle)value;
		}
		else if (t == typeof(ConjugateHandle))
		{
			Conjugate = (ConjugateHandle)value;
		}
		else if (t.IsEnum)
		{
			// @FIXME
			Object = Convert.ChangeType(value, t.GetEnumUnderlyingType());
		}
		else
		{
			Thrower.NoEntry();
		}
	}

	public EZCallBufferSlotType Type => _type;

	public uint8 UInt8
	{
		readonly get => ReadUInt8();
		set => WriteUInt8(value);
	}
	
	public uint16 UInt16
	{
		readonly get => ReadUInt16();
		set => WriteUInt16(value);
	}
	
	public uint32 UInt32
	{
		readonly get => ReadUInt32();
		set => WriteUInt32(value);
	}
	
	public uint64 UInt64
	{
		readonly get => ReadUInt64();
		set => WriteUInt64(value);
	}
	
	public int8 Int8
	{
		readonly get => ReadInt8();
		set => WriteInt8(value);
	}
	
	public int16 Int16
	{
		readonly get => ReadInt16();
		set => WriteInt16(value);
	}
	
	public int32 Int32
	{
		readonly get => ReadInt32();
		set => WriteInt32(value);
	}
	
	public int64 Int64
	{
		readonly get => ReadInt64();
		set => WriteInt64(value);
	}
	
	public float Float
	{
		readonly get => ReadFloat();
		set => WriteDouble(value);
	}
	
	public double Double
	{
		readonly get => ReadDouble();
		set => WriteDouble(value);
	}
	
	public bool Bool
	{
		readonly get => ReadBool();
		set => WriteBool(value);
	}
	
	public IntPtr Pointer
	{
		readonly get => ReadPointer();
		set => WritePointer(value);
	}
	
	public GCHandle GCHandle
	{
		readonly get => ReadGCHandle();
		set => WriteGCHandle(value);
	}
	
	public ConjugateHandle Conjugate
	{
		readonly get => ReadConjugate();
		set => WriteConjugate(value);
	}

	public object? Object
	{
		readonly get => ReadObject();
		set => WriteObject(value);
	}

	private ZCallBufferSlot(EZCallBufferSlotType type) => _type = type;
	
	private readonly EZCallBufferSlotType _type;
	private ZCallBufferSlotValue _value;
	
}

[StructLayout(LayoutKind.Sequential)]
public readonly unsafe struct ZCallBuffer(ZCallBufferSlot* slots, int32 numSlots)
{
	
	public ref ZCallBufferSlot this[int32 index]
	{
		get
		{
			if (index < 0)
			{
				index = _numSlots + index;
			}

			if (index < 0 || index >= _numSlots)
			{
				throw new IndexOutOfRangeException();
			}

			return ref _slots[index];
		}
	}

	public int32 NumSlots => _numSlots;
	
    private readonly ZCallBufferSlot* _slots = slots;
    private readonly int32 _numSlots = numSlots;
    
}


