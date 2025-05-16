// Copyright Zero Games. All Rights Reserved.

using System.Numerics;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public sealed class TStrongObjectPtr<T> : StrongObjectPtrBase
	, ICloneable<TStrongObjectPtr<T>>
	, IEquatable<TStrongObjectPtr<T>>
	, IEqualityOperators<TStrongObjectPtr<T>?, TStrongObjectPtr<T>?, bool>
	, IUnrealObjectWrapper<T>
	where T : UObject
{

	public static TStrongObjectPtr<T> From<TSource>(TStrongObjectPtr<TSource> other) where TSource : T => new(other.Target);

	public TStrongObjectPtr() : this(null){}
	public TStrongObjectPtr(T? target) : base(target){}

	public TStrongObjectPtr<T> Clone() => new(Target);
	object ICloneable.Clone() => Clone();

	public bool Equals(TStrongObjectPtr<T>? other) => base.Equals(other);
	public override bool Equals(object? obj) => base.Equals(obj);
	public override int32 GetHashCode() => base.GetHashCode();
	
	public static implicit operator TStrongObjectPtr<T>(T? target) => new(target);
	public static bool operator ==(TStrongObjectPtr<T>? left, TStrongObjectPtr<T>? right) => Equals(left, right);
	public static bool operator !=(TStrongObjectPtr<T>? left, TStrongObjectPtr<T>? right) => !Equals(left, right);
	
	public static IEqualityComparer<TStrongObjectPtr<T>> DefaultEqualityComparer { get; } = new EqualityComparer();

	public new T? Target
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalGet(false);
		}
		set
		{
			MasterAlcCache.GuardInvariant();
			InternalSet(value);
		}
	}

	public new T? TargetEvenIfGarbage
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalGet(true);
		}
	}

	protected override UObject? InternalGetTarget(bool evenIfGarbage) => evenIfGarbage ? TargetEvenIfGarbage : Target;

	protected override void InternalSetTarget(UObject? target)
	{
		if (target is not null && target is not T)
		{
			throw new ArgumentOutOfRangeException(nameof(target));
		}

		Target = (T?)target;
	}

	private sealed class EqualityComparer : IEqualityComparer<TStrongObjectPtr<T>>
	{
		public bool Equals(TStrongObjectPtr<T>? lhs, TStrongObjectPtr<T>? rhs) => lhs == rhs;
		public int32 GetHashCode(TStrongObjectPtr<T> obj) => obj.GetHashCode();
	}

	private unsafe T? InternalGet(bool evenIfGarbage)
	{
		Thrower.ThrowIfNotInGameThread();
		MasterAlcCache.Instance.GuardUnloaded();
		return StrongObjectPtr_Interop.Get(Unmanaged, Convert.ToByte(evenIfGarbage)).GetTarget<T>();
	}

	private unsafe void InternalSet(T? target)
	{
		Thrower.ThrowIfNotInGameThread();
		MasterAlcCache.Instance.GuardUnloaded();
		StrongObjectPtr_Interop.Set(Unmanaged, ConjugateHandle.FromConjugate(target));
	}
	
}


