// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

public readonly unsafe struct InteropStringArray : IDisposable
{
	
	public InteropStringArray() : this(null) {}

	public InteropStringArray(IEnumerable<string?>? strings)
	{
		_black = true;
		_address = InteropStringArray_Interop.Alloc();
		foreach (var str in strings ?? [])
		{
			Add(str);
		}
	}

	public InteropStringArray(IntPtr address)
	{
		_address = address;
	}

	public void Dispose()
	{
		if (_black)
		{
			InteropStringArray_Interop.Free(_address);
		}
	}

	public void Add(string? value)
	{
		if (_address == IntPtr.Zero)
		{
			return;
		}
		
		fixed (char* buffer = value)
		{
			InteropStringArray_Interop.Add(_address, buffer);
		}
	}

	public void InsertAt(int32 index, string? value)
	{
		if (_address == IntPtr.Zero)
		{
			return;
		}

		fixed (char* buffer = value)
		{
			InteropStringArray_Interop.InsertAt(_address, index, buffer);
		}
	}

	public void RemoveAt(int32 index)
	{
		if (_address == IntPtr.Zero)
		{
			return;
		}

		InteropStringArray_Interop.RemoveAt(_address, index);
	}

	public void Clear()
	{
		if (_address == IntPtr.Zero)
		{
			return;
		}
		
		InteropStringArray_Interop.Clear(_address);
	}

	public string[] ToStringArray()
	{
		int32 count = Count;
		string[] result = count > 0 ? new string[count] : [];
		for (int32 i = 0; i < count; ++i)
		{
			result[i] = this[i];
		}

		return result;
	}

	public string this[int32 index]
	{
		get => _address != IntPtr.Zero ? new(InteropStringArray_Interop.Get(_address, index)) : string.Empty;
		set
		{
			if (_address == IntPtr.Zero)
			{
				return;
			}
			
			fixed (char* buffer = value)
			{
				InteropStringArray_Interop.Set(_address, index, buffer);
			}
		}
	}

	public int32 Count => _address != IntPtr.Zero ? InteropStringArray_Interop.Count(_address) : 0;

	public IntPtr Address => _address;

	private readonly bool _black;
	private readonly IntPtr _address;
	
}