// Copyright Zero Games. All Rights Reserved.

using System.Numerics;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
[ConjugateRegistryId(24)]
public sealed class WeakObjectPtr<T> : WeakObjectPtrBase
	, IConjugate<WeakObjectPtr<T>>
	, ICloneable<WeakObjectPtr<T>>
	, IEquatable<WeakObjectPtr<T>>
	, IEqualityOperators<WeakObjectPtr<T>?, WeakObjectPtr<T>?, bool>
	, IWeakObjectWrapper<T>
	where T : UnrealObject
{

	public static WeakObjectPtr<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);
	
	public static WeakObjectPtr<T> From<TSource>(WeakObjectPtr<TSource> other) where TSource : T => new(other);

	public WeakObjectPtr()
	{
		Unmanaged = MasterAlcCache.Instance.BuildConjugate(this, GetStaticClass(typeof(T)).Unmanaged);
	}
	
	public WeakObjectPtr(T? target) : this()
	{
		if (target is null)
		{
			return;
		}

		Target = target;
	}
	
	public WeakObjectPtr<T> Clone() => new(this);
	object ICloneable.Clone() => Clone();

	public bool Equals(WeakObjectPtr<T>? other) => base.Equals(other);
	public override bool Equals(object? obj) => base.Equals(obj);
	public override int32 GetHashCode() => base.GetHashCode();
	
	public static implicit operator WeakObjectPtr<T>(T? target) => new(target);
	public static bool operator ==(WeakObjectPtr<T>? left, WeakObjectPtr<T>? right) => Equals(left, right);
	public static bool operator !=(WeakObjectPtr<T>? left, WeakObjectPtr<T>? right) => !Equals(left, right);
	
	public static IEqualityComparer<WeakObjectPtr<T>> DefaultEqualityComparer { get; } = new EqualityComparer();

	public T? Target
	{
		get => InternalGet(false);
		set => InternalSet(value);
	}

	public T? TargetEvenIfGarbage => InternalGet(true);

	public bool IsValid => InternalIsValid(false);
	public bool IsValidEventIfGarbage => InternalIsValid(true);
	public bool IsNull => InternalIsNull();
	public bool IsStale => InternalIsStale(true);
	public bool IsStaleExcludingGarbage => InternalIsStale(false);
	
	private sealed class EqualityComparer : IEqualityComparer<WeakObjectPtr<T>>
	{
		public bool Equals(WeakObjectPtr<T>? lhs, WeakObjectPtr<T>? rhs) => lhs == rhs;
		public int32 GetHashCode(WeakObjectPtr<T> obj) => obj.GetHashCode();
	}

	private WeakObjectPtr(IntPtr unmanaged) : base(unmanaged){}
	private WeakObjectPtr(WeakObjectPtrBase? other) : this() => InternalCopy(other);

	private unsafe void InternalCopy(WeakObjectPtrBase? other)
	{
		Thrower.ThrowIfNotInGameThread();
		
		if (other is not null)
		{
			WeakObjectPtr_Interop.Copy(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(other));
		}
		else
		{
			Target = null;
		}
	}

	private unsafe T? InternalGet(bool evenIfGarbage)
	{
		Thrower.ThrowIfNotInGameThread();
		return WeakObjectPtr_Interop.Get(ConjugateHandle.FromConjugate(this), Convert.ToByte(evenIfGarbage)).GetTarget<T>();
	}

	private unsafe void InternalSet(T? target)
	{
		Thrower.ThrowIfNotInGameThread();
		WeakObjectPtr_Interop.Set(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(target));
	}
	
	private unsafe bool InternalIsValid(bool evenIfGarbage)
	{
		Thrower.ThrowIfNotInGameThread();
		return WeakObjectPtr_Interop.IsValid(ConjugateHandle.FromConjugate(this), Convert.ToByte(evenIfGarbage)) > 0;
	}

	private unsafe bool InternalIsNull()
	{
		Thrower.ThrowIfNotInGameThread();
		return WeakObjectPtr_Interop.IsNull(ConjugateHandle.FromConjugate(this)) > 0;
	}

	private unsafe bool InternalIsStale(bool includingGarbage)
	{
		Thrower.ThrowIfNotInGameThread();
		return WeakObjectPtr_Interop.IsStale(ConjugateHandle.FromConjugate(this), Convert.ToByte(includingGarbage)) > 0;
	}
	
}


