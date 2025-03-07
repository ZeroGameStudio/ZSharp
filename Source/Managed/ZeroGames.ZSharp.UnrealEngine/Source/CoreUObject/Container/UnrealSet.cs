// Copyright Zero Games. All Rights Reserved.

using System.Collections;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
[ConjugateRegistryId(32)]
[ConjugateKey("Unreal.Set")]
public sealed class UnrealSet<T> : UnrealConjugateBase
	, IConjugate<UnrealSet<T>>
	, ISet<T>
	, IReadOnlySet<T>
	, ICloneable<UnrealSet<T>>
{
	
    public struct Enumerator : IEnumerator<T>
    {
	    public bool MoveNext()
	    {
		    GuardInvariant();
		    MasterAlcCache.GuardInvariant();
		    if (_unmanaged == default)
		    {
			    return false;
		    }

		    bool first = _first;
		    _first = false;
		    return first || InternalMoveNext();
	    }

	    public void Reset()
	    {
		    GuardInvariant();
		    MasterAlcCache.GuardInvariant();

		    TryRelease();
		    _first = true;
		    _unmanaged = InternalCreate();
	    }

	    public void Dispose()
	    {
		    TryRelease();
		    _target = null;
	    }
        
	    public T Current
	    {
		    get
		    {
			    GuardInvariant();
			    MasterAlcCache.GuardInvariant();
			    if (_unmanaged == default)
			    {
				    throw new InvalidOperationException();
			    }
			    
			    return InternalCurrent;
		    }
	    }
	    object? IEnumerator.Current => Current;

	    internal Enumerator(UnrealSet<T> target)
	    {
		    _target = target;
		    _snapshot = target.Count;
		    _first = true;
		    _unmanaged = InternalCreate();
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

	    private void TryRelease()
	    {
		    if (_unmanaged != default)
		    {
			    InternalRelease();
			    _unmanaged = default;
		    }
	    }

	    private unsafe IntPtr InternalCreate() => UnrealSet_Interop.CreateEnumerator(ConjugateHandle.FromConjugate(_target));
	    private unsafe void InternalRelease() => UnrealSet_Interop.ReleaseEnumerator(_unmanaged);
	    private unsafe bool InternalMoveNext() => UnrealSet_Interop.EnumeratorMoveNext(_unmanaged) > 0;

	    private unsafe T InternalCurrent
	    {
		    get
		    {
			    ZCallBufferSlot current = ZCallBufferSlot.FromType(typeof(T));
			    UnrealSet_Interop.EnumeratorCurrent(_unmanaged, ConjugateHandle.FromConjugate(_target), ref current);
			    return (T)current.Object!;
		    }
	    }

	    private UnrealSet<T>? _target;
	    private readonly int32 _snapshot;
	    private bool _first;
	    private IntPtr _unmanaged;
    }
	
	public static UnrealSet<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);

	public UnrealSet()
	{
		if (!PropertyHelper.CanBeKey(typeof(T)) || !PropertyHelper.CanBeValue(typeof(T)))
		{
			throw new NotSupportedException($"Element type {typeof(T).FullName} is not supported.");
		}
		
		InternalConstruct();
	}

	public UnrealSet(IEnumerable<T> source) : this()
	{
		foreach (var item in source)
		{
			Add(item);
		}
	}
	
	public Enumerator GetEnumerator() => new(this);
	IEnumerator<T> IEnumerable<T>.GetEnumerator() => GetEnumerator();
	IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();

	public bool Add(T item)
	{
		MasterAlcCache.GuardInvariant();
		return InternalAdd(item);
	}
	void ICollection<T>.Add(T item) => Add(item);

	public bool Remove(T item)
	{
		MasterAlcCache.GuardInvariant();
		return InternalRemove(item);
	}

	public void Clear()
	{
		MasterAlcCache.GuardInvariant();
		InternalClear();
	}

	public bool Contains(T item)
	{
		MasterAlcCache.GuardInvariant();
		return InternalContains(item);
	}

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

	// UnrealSet is for interop purpose, convert to HashSet if you need these.
	void ISet<T>.UnionWith(IEnumerable<T> other) => throw new NotSupportedException();
	void ISet<T>.IntersectWith(IEnumerable<T> other) => throw new NotSupportedException();
	void ISet<T>.ExceptWith(IEnumerable<T> other) => throw new NotSupportedException();
	void ISet<T>.SymmetricExceptWith(IEnumerable<T> other) => throw new NotSupportedException();
	bool ISet<T>.IsSupersetOf(IEnumerable<T> other) => throw new NotSupportedException();
	bool IReadOnlySet<T>.IsSupersetOf(IEnumerable<T> other) => throw new NotSupportedException();
	bool ISet<T>.IsProperSupersetOf(IEnumerable<T> other) => throw new NotSupportedException();
	bool IReadOnlySet<T>.IsProperSupersetOf(IEnumerable<T> other) => throw new NotSupportedException();
	bool ISet<T>.IsSubsetOf(IEnumerable<T> other) => throw new NotSupportedException();
	bool IReadOnlySet<T>.IsSubsetOf(IEnumerable<T> other) => throw new NotSupportedException();
	bool ISet<T>.IsProperSubsetOf(IEnumerable<T> other) => throw new NotSupportedException();
	bool IReadOnlySet<T>.IsProperSubsetOf(IEnumerable<T> other) => throw new NotSupportedException();
	bool ISet<T>.Overlaps(IEnumerable<T> other) => throw new NotSupportedException();
	bool IReadOnlySet<T>.Overlaps(IEnumerable<T> other) => throw new NotSupportedException();
	bool ISet<T>.SetEquals(IEnumerable<T> other) => throw new NotSupportedException();
	bool IReadOnlySet<T>.SetEquals(IEnumerable<T> other) => throw new NotSupportedException();
	
	public UnrealSet<T> Clone() => new(this);
	object ICloneable.Clone() => Clone();

	public int32 Count
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalCount;
		}
	}

	bool ICollection<T>.IsReadOnly => false;
	
	[StructLayout(LayoutKind.Sequential)]
	private struct Userdata
	{
		public PropertyDesc ElementProperty;
	}

	private UnrealSet(IntPtr unmanaged) : base(unmanaged){}
	
	private unsafe void InternalConstruct()
	{
		Userdata userdata = new();
		Userdata* pUserdata = &userdata;
		PropertyHelper.TryGetPropertyDesc(typeof(T), out userdata.ElementProperty);
		
		BuildConjugate_Black((IntPtr)pUserdata);
	}

	private unsafe bool InternalAdd(T item) => UnrealSet_Interop.Add(ConjugateHandle.FromConjugate(this), ZCallBufferSlot.FromObject(item)) > 0;
	private unsafe bool InternalRemove(T item) => UnrealSet_Interop.Remove(ConjugateHandle.FromConjugate(this), ZCallBufferSlot.FromObject(item)) > 0;
	private unsafe void InternalClear() => UnrealSet_Interop.Clear(ConjugateHandle.FromConjugate(this));
	private unsafe bool InternalContains(T item) => UnrealSet_Interop.Contains(ConjugateHandle.FromConjugate(this), ZCallBufferSlot.FromObject(item)) > 0;
	private unsafe int32 InternalCount => UnrealSet_Interop.Count(ConjugateHandle.FromConjugate(this));

}


