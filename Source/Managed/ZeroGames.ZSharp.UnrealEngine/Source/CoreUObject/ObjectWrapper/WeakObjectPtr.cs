// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
public sealed class WeakObjectPtr<T> : WeakObjectPtrBase
	, IConjugate<WeakObjectPtr<T>>
	, ICloneable<WeakObjectPtr<T>>
	where T : UnrealObject
{
	
	public static WeakObjectPtr<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);
	
	public static WeakObjectPtr<T> From<TSource>(WeakObjectPtr<TSource> other) where TSource : T => new(other);

	public WeakObjectPtr()
	{
		Unmanaged = MasterAlcCache.Instance.BuildConjugate(this, GetStaticClass(typeof(T)).Unmanaged);
	}
	
	public WeakObjectPtr(T? obj) : this()
	{
		if (obj is null)
		{
			return;
		}

		Object = obj;
	}
	
	public WeakObjectPtr<T> Clone() => new(this);
	object ICloneable.Clone() => Clone();
	
	public static implicit operator WeakObjectPtr<T>(T? obj) => new(obj);

	public T? Object
	{
		get => InternalGet(false);
		set => InternalSet(value);
	}
	
	public T? ObjectEvenIfGarbage => InternalGet(true);

	private WeakObjectPtr(IntPtr unmanaged) : base(unmanaged){}
	private WeakObjectPtr(WeakObjectPtrBase other) : this() => InternalCopy(other);

	private unsafe void InternalCopy(WeakObjectPtrBase other)
	{
		Thrower.ThrowIfNotInGameThread();
		WeakObjectPtr_Interop.Copy(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(other));
	}

	private unsafe T? InternalGet(bool evenIfGarbage)
	{
		Thrower.ThrowIfNotInGameThread();
		return WeakObjectPtr_Interop.Get(ConjugateHandle.FromConjugate(this), Convert.ToByte(evenIfGarbage)).GetTarget<T>();
	}

	private unsafe void InternalSet(T? obj)
	{
		Thrower.ThrowIfNotInGameThread();
		WeakObjectPtr_Interop.Set(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(obj));
	}
	
}


