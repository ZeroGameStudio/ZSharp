// Copyright Zero Games. All Rights Reserved.

using System.Numerics;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public sealed class StrongObjectPtr<T> : StrongObjectPtrBase
	, ICloneable<StrongObjectPtr<T>>
	, IEquatable<StrongObjectPtr<T>>
	, IEqualityOperators<StrongObjectPtr<T>?, StrongObjectPtr<T>?, bool>
	, IUnrealObjectWrapper<T>
	where T : UnrealObject
{

	public static StrongObjectPtr<T> From<TSource>(StrongObjectPtr<TSource> other) where TSource : T => new(other.Target);

	public StrongObjectPtr() : this(null){}
	public StrongObjectPtr(T? target) : base(target){}

	public StrongObjectPtr<T> Clone() => new(Target);
	object ICloneable.Clone() => Clone();

	public bool Equals(StrongObjectPtr<T>? other) => base.Equals(other);
	public override bool Equals(object? obj) => base.Equals(obj);
	public override int32 GetHashCode() => base.GetHashCode();
	
	public static implicit operator StrongObjectPtr<T>(T? target) => new(target);
	public static bool operator ==(StrongObjectPtr<T>? left, StrongObjectPtr<T>? right) => Equals(left, right);
	public static bool operator !=(StrongObjectPtr<T>? left, StrongObjectPtr<T>? right) => !Equals(left, right);
	
	public static IEqualityComparer<StrongObjectPtr<T>> DefaultEqualityComparer { get; } = new EqualityComparer();

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

	protected override UnrealObject? InternalGetTarget(bool evenIfGarbage) => evenIfGarbage ? TargetEvenIfGarbage : Target;

	protected override void InternalSetTarget(UnrealObject? target)
	{
		if (target is not null && target is not T)
		{
			throw new ArgumentOutOfRangeException(nameof(target));
		}

		Target = (T?)target;
	}

	private sealed class EqualityComparer : IEqualityComparer<StrongObjectPtr<T>>
	{
		public bool Equals(StrongObjectPtr<T>? lhs, StrongObjectPtr<T>? rhs) => lhs == rhs;
		public int32 GetHashCode(StrongObjectPtr<T> obj) => obj.GetHashCode();
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


