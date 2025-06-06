﻿// Copyright Zero Games. All Rights Reserved.

using System.Collections;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
[ConjugateRegistryId(31)]
[ConjugateKey("Unreal.Array")]
public sealed class TArray<T> : UnrealConjugateBase
	, IConjugate<TArray<T>>
	, IList
	, IList<T>
	, IReadOnlyList<T>
	, ISliceable<TArray<T>>
	, ICloneable<TArray<T>>
{
	
    public struct Enumerator : IEnumerator<T>
    {
	    public bool MoveNext()
	    {
		    GuardInvariant();
		    int32 index = _index + 1;
		    _index = Math.Min(index, _snapshot);
		    return _index < _snapshot;
	    }
        
	    public void Reset() => _index = -1;

	    public void Dispose() => _target = null;
        
	    public T Current
	    {
		    get
		    {
			    GuardInvariant();
			    return _target![_index];
		    }
	    }
	    object? IEnumerator.Current => Current;

	    internal Enumerator(TArray<T> target)
	    {
		    _target = target;
		    _snapshot = target.Count;
	    }

	    private void GuardInvariant()
	    {
		    if (_target is null)
		    {
			    throw new InvalidOperationException("Access enumerator after disposal.");
		    }

		    if (_target.IsExpired)
		    {
			    throw new InvalidOperationException("Target enumerable has expired.");
		    }

		    if (_target.Count != _snapshot)
		    {
			    throw new InvalidOperationException("Target enumerable has changed.");
		    }
	    }
        
	    private TArray<T>? _target;
	    private readonly int32 _snapshot;
	    private int32 _index = -1;
    }

	public static TArray<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);

	public TArray()
	{
		if (!PropertyHelper.CanBeValue(typeof(T)))
		{
			NotifyInitializationFailed(new NotSupportedException($"Element type {typeof(T).FullName} is not supported."));
		}
		
		InternalConstruct();
	}

	public TArray(IEnumerable<T> source) : this() => AddRange(source);

	public Enumerator GetEnumerator() => new(this);
	IEnumerator<T> IEnumerable<T>.GetEnumerator() => GetEnumerator();
	IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();

	public void Add(T item) => Insert(Count, item);
	int32 IList.Add(object? value)
	{
		Add((T)value!);
		return Count - 1;
	}

	public void AddRange(IEnumerable<T> items) => InsertRange(Count, items);

	public void Insert(int32 index, T item)
	{
		MasterAlcCache.GuardInvariant();

		// index == Count is allowed for insert.
		if (index < 0 || index > Count)
		{
			throw new IndexOutOfRangeException();
		}
		
		InternalInsert(index, item);
	}
	void IList.Insert(int32 index, object? value) => Insert(index, (T)value!);

	public void InsertRange(int32 index, IEnumerable<T> items)
	{
		MasterAlcCache.GuardInvariant();
		GuardIndex(index);
		foreach (var item in items)
		{
			InternalInsert(index, item);
		}
	}
	
	public bool Remove(T item)
	{
		MasterAlcCache.GuardInvariant();
		return InternalRemove(item);
	}
	void IList.Remove(object? value) => Remove((T)value!);
	
	public void RemoveAt(int32 index)
	{
		MasterAlcCache.GuardInvariant();
		GuardIndex(index);
		InternalRemoveAt(index);
	}

	public void Clear()
	{
		MasterAlcCache.GuardInvariant();
		InternalClear();
	}
	
	public int32 IndexOf(T item)
	{
		MasterAlcCache.GuardInvariant();
		return InternalIndexOf(item);
	}
	int32 IList.IndexOf(object? value) => IndexOf((T)value!);
	
	public bool Contains(T item) => IndexOf(item) >= 0;
	bool IList.Contains(object? value) => Contains((T)value!);

	public void CopyTo(T[] array, int32 arrayIndex)
	{
		if (arrayIndex < 0)
		{
			throw new ArgumentOutOfRangeException(nameof(arrayIndex));
		}

		if (Count > array.Length - arrayIndex)
		{
			throw new ArgumentException();
		}

		int32 i = arrayIndex;
		foreach (var item in this)
		{
			array[i++] = item;
		}
	}
	void ICollection.CopyTo(Array array, int32 index) => CopyTo((T[])array, index);

	public TArray<T> Clone() => new(this);
	object ICloneable.Clone() => Clone();
	
	public TArray<T> Slice(int32 start, int32 length)
	{
		if (length < 0)
		{
			throw new ArgumentOutOfRangeException(nameof(length));
		}
		
		GuardIndex(start);
		GuardIndex(start + length - 1);
		TArray<T> result = new();
		for (int32 i = start; i < start + length; ++i)
		{
			result.Add(this[i]);
		}
		return result;
	}
	
	public bool IsValidIndex(int32 index) => index >= 0 && index < Count;
	
	public T this[int32 index]
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			GuardIndex(index);
			return InternalGet(index);
		}
		set
		{
			MasterAlcCache.GuardInvariant();
			GuardIndex(index);
			InternalSet(index, value);
		}
	}
	object? IList.this[int index]
	{
		get => this[index];
		set => this[index] = (T)value!;
	}

	public int32 Count
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalCount;
		}
	}

	bool ICollection<T>.IsReadOnly => false;
	bool ICollection.IsSynchronized => false;
	object ICollection.SyncRoot => this;
	
	bool IList.IsReadOnly => false;
	bool IList.IsFixedSize => false;
	
	[StructLayout(LayoutKind.Sequential)]
	private struct Userdata
	{
		public PropertyDesc ElementProperty;
	}

	private TArray(IntPtr unmanaged) : base(unmanaged){}

	private void GuardIndex(int32 index)
	{
		if (!IsValidIndex(index))
		{
			throw new IndexOutOfRangeException();
		}
	}
	
	private unsafe void InternalConstruct()
	{
		Userdata userdata = new();
		Userdata* pUserdata = &userdata;
		PropertyHelper.TryGetPropertyDesc(typeof(T), out userdata.ElementProperty);
		
		BuildConjugate_Black((IntPtr)pUserdata);
	}

	private unsafe void InternalInsert(int32 index, T item) => UnrealArray_Interop.Insert(ConjugateHandle.FromConjugate(this), index, ZCallBufferSlot.FromObject(item));
	private unsafe bool InternalRemove(T item) => UnrealArray_Interop.Remove(ConjugateHandle.FromConjugate(this), ZCallBufferSlot.FromObject(item)) > 0;
	private unsafe void InternalRemoveAt(int32 index) => UnrealArray_Interop.RemoveAt(ConjugateHandle.FromConjugate(this), index);
	private unsafe void InternalClear() => UnrealArray_Interop.Clear(ConjugateHandle.FromConjugate(this));
	private unsafe int32 InternalIndexOf(T item) => UnrealArray_Interop.IndexOf(ConjugateHandle.FromConjugate(this), ZCallBufferSlot.FromObject(item));

	private unsafe T InternalGet(int32 index)
	{
		ZCallBufferSlot item = ZCallBufferSlot.FromType(typeof(T));
		UnrealArray_Interop.Get(ConjugateHandle.FromConjugate(this), index, ref item);
		return (T)item.Object!;
	}
	
	private unsafe void InternalSet(int32 index, T item) => UnrealArray_Interop.Set(ConjugateHandle.FromConjugate(this), index, ZCallBufferSlot.FromObject(item));
	
	private unsafe int32 InternalCount => UnrealArray_Interop.Count(ConjugateHandle.FromConjugate(this));
	
}


