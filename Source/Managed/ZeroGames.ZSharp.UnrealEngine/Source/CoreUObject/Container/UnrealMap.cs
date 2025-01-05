// Copyright Zero Games. All Rights Reserved.

using System.Collections;
using System.Diagnostics.CodeAnalysis;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
[ConjugateRegistryId(33)]
[ConjugateKey("Unreal.Map")]
public sealed class UnrealMap<TKey, TValue> : UnrealConjugateBase
	, IConjugate<UnrealMap<TKey, TValue>>
	, IDictionary
	, IDictionary<TKey, TValue>
	, IReadOnlyDictionary<TKey, TValue>
	, ICloneable<UnrealMap<TKey, TValue>>
	where TKey : notnull
{
	
	public struct Enumerator : IEnumerator<KeyValuePair<TKey, TValue>>, IDictionaryEnumerator
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
        
	    public KeyValuePair<TKey, TValue> Current
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
	    object IEnumerator.Current => Current;
	    
	    object IDictionaryEnumerator.Key => Current.Key!;
	    object? IDictionaryEnumerator.Value => Current.Value;
	    DictionaryEntry IDictionaryEnumerator.Entry => new(Current.Key!, Current.Value);

	    internal Enumerator(UnrealMap<TKey, TValue> target)
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

	    private unsafe IntPtr InternalCreate() => UnrealMap_Interop.CreateEnumerator(ConjugateHandle.FromConjugate(_target));
	    private unsafe void InternalRelease() => UnrealMap_Interop.ReleaseEnumerator(_unmanaged);
	    private unsafe bool InternalMoveNext() => UnrealMap_Interop.EnumeratorMoveNext(_unmanaged) > 0;

	    private unsafe KeyValuePair<TKey, TValue> InternalCurrent
	    {
		    get
		    {
			    ZCallBufferSlot currentKey = ZCallBufferSlot.FromType(typeof(TKey));
			    ZCallBufferSlot currentValue = ZCallBufferSlot.FromType(typeof(TValue));
			    UnrealMap_Interop.EnumeratorCurrent(_unmanaged, ConjugateHandle.FromConjugate(_target), ref currentKey, ref currentValue);
			    return new((TKey)currentKey.Object!, (TValue)currentValue.Object!);
		    }
	    }

	    private UnrealMap<TKey, TValue>? _target;
	    private readonly int32 _snapshot;
	    private bool _first;
	    private IntPtr _unmanaged;
    }
	
	public static UnrealMap<TKey, TValue> BuildConjugate(IntPtr unmanaged) => new(unmanaged);

	public UnrealMap()
	{
		if (!PropertyHelper.CanBeValue(typeof(TKey)) || !PropertyHelper.CanBeValue(typeof(TValue)))
		{
			Unmanaged = DEAD_ADDR;
			GC.SuppressFinalize(this);
			throw new NotSupportedException($"Element type {typeof(TKey).FullName} or {typeof(TValue).FullName} is not supported.");
		}
		
		InternalConstruct();
	}

	public UnrealMap(IEnumerable<KeyValuePair<TKey, TValue>> source) : this()
	{
		foreach (var pair in source)
		{
			Add(pair.Key, pair.Value);
		}
	}
	
	public Enumerator GetEnumerator() => new(this);
	IDictionaryEnumerator IDictionary.GetEnumerator() => GetEnumerator();
	IEnumerator<KeyValuePair<TKey, TValue>> IEnumerable<KeyValuePair<TKey, TValue>>.GetEnumerator() => GetEnumerator();
	IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();

	public bool TryAdd(TKey key, TValue value)
	{
		MasterAlcCache.GuardInvariant();
		return InternalAdd(key, value);
	}

	public void Add(TKey key, TValue value)
	{
		MasterAlcCache.GuardInvariant();
		InternalSet(key, value);
	}
	public void Add(KeyValuePair<TKey, TValue> item) => Add(item.Key, item.Value);
	void IDictionary.Add(object key, object? value) => Add((TKey)key, (TValue)value!);
	void ICollection<KeyValuePair<TKey,TValue>>.Add(KeyValuePair<TKey, TValue> item) => Add(item.Key, item.Value);

	public bool Remove(TKey key)
	{
		MasterAlcCache.GuardInvariant();
		return InternalRemove(key);
	}
	void IDictionary.Remove(object key) => Remove((TKey)key);
	bool ICollection<KeyValuePair<TKey,TValue>>.Remove(KeyValuePair<TKey, TValue> item) => Remove(item.Key);

	public void Clear()
	{
		MasterAlcCache.GuardInvariant();
		InternalClear();
	}
	
	public bool ContainsKey(TKey key)
	{
		MasterAlcCache.GuardInvariant();
		return InternalContainsKey(key);
	}
	bool IDictionary.Contains(object key) => ContainsKey((TKey)key);
	bool ICollection<KeyValuePair<TKey, TValue>>.Contains(KeyValuePair<TKey, TValue> item) => ContainsKey(item.Key);

	public bool TryGetValue(TKey key, [MaybeNullWhen(false)] out TValue value)
	{
		MasterAlcCache.GuardInvariant();
		return InternalGet(key, out value);
	}

	public void CopyTo(KeyValuePair<TKey, TValue>[] array, int32 arrayIndex)
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
		foreach (var pair in this)
		{
			array[i++] = pair;
		}
	}
	void ICollection.CopyTo(Array array, int32 index) => CopyTo((KeyValuePair<TKey, TValue>[])array, index);
	
	public UnrealMap<TKey, TValue> Clone() => new(this);
	object ICloneable.Clone() => Clone();

	public TValue this[TKey key]
	{
		get
		{
			if (!TryGetValue(key, out var value))
			{
				throw new KeyNotFoundException();
			}

			return value;
		}
		set => Add(key, value);
	}
	object? IDictionary.this[object key]
	{
		get => this[(TKey)key];
		set => this[(TKey)key] = (TValue)value!; 
	}

	public int32 Count
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalCount;
		}
	}

	public TKey[] Keys
	{
		get
		{
			TKey[] keys = new TKey[Count];
			int32 index = 0;
			foreach (var pair in this)
			{
				keys[index++] = pair.Key;
			}
			return keys;
		}
	}
	ICollection<TKey> IDictionary<TKey,TValue>.Keys => Keys;
	IEnumerable<TKey> IReadOnlyDictionary<TKey, TValue>.Keys => Keys;
	ICollection IDictionary.Keys => Keys;

	public TValue[] Values
	{
		get
		{
			TValue[] values = new TValue[Count];
			int32 index = 0;
			foreach (var pair in this)
			{
				values[index++] = pair.Value;
			}
			return values;
		}
	}
	ICollection<TValue> IDictionary<TKey, TValue>.Values => Values;
	IEnumerable<TValue> IReadOnlyDictionary<TKey, TValue>.Values => Values;
	ICollection IDictionary.Values => Values;

	bool ICollection<KeyValuePair<TKey, TValue>>.IsReadOnly => false;
	bool ICollection.IsSynchronized => false;
	object ICollection.SyncRoot => this;
	
	bool IDictionary.IsReadOnly => false;
	bool IDictionary.IsFixedSize => false;
	
	[StructLayout(LayoutKind.Sequential)]
	private struct Userdata
	{
		public PropertyDesc KeyProperty;
		public PropertyDesc ValueProperty;
	}
	
	private UnrealMap(IntPtr unmanaged) : base(unmanaged){}
	
	private unsafe void InternalConstruct()
	{
		Userdata userdata = new();
		Userdata* pUserdata = &userdata;
		PropertyHelper.TryGetPropertyDesc(typeof(TKey), out userdata.KeyProperty);
		PropertyHelper.TryGetPropertyDesc(typeof(TValue), out userdata.ValueProperty);
		
		Unmanaged = MasterAlcCache.Instance.BuildConjugate(this, (IntPtr)pUserdata);
	}
	
	private unsafe bool InternalAdd(TKey key, TValue value) => UnrealMap_Interop.TryAdd(ConjugateHandle.FromConjugate(this), ZCallBufferSlot.FromObject(key), ZCallBufferSlot.FromObject(value)) > 0;
	private unsafe bool InternalRemove(TKey key) => UnrealMap_Interop.Remove(ConjugateHandle.FromConjugate(this), ZCallBufferSlot.FromObject(key)) > 0;
	private unsafe void InternalClear() => UnrealMap_Interop.Clear(ConjugateHandle.FromConjugate(this));
	private unsafe bool InternalContainsKey(TKey key) => UnrealMap_Interop.Contains(ConjugateHandle.FromConjugate(this), ZCallBufferSlot.FromObject(key)) > 0;

	private unsafe bool InternalGet(TKey key, [MaybeNullWhen(false)] out TValue value)
	{
		ZCallBufferSlot result = ZCallBufferSlot.FromType(typeof(TValue));
		if (UnrealMap_Interop.Get(ConjugateHandle.FromConjugate(this), ZCallBufferSlot.FromObject(key), ref result) > 0)
		{
			value = (TValue)result.Object!;
			return true;
		}

		value = default;
		return false;
	}
	
	private unsafe void InternalSet(TKey key, TValue value) => UnrealMap_Interop.Set(ConjugateHandle.FromConjugate(this), ZCallBufferSlot.FromObject(key), ZCallBufferSlot.FromObject(value));
	
	private unsafe int32 InternalCount => UnrealMap_Interop.Count(ConjugateHandle.FromConjugate(this));

}


