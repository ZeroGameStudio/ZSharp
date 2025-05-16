// Copyright Zero Games. All Rights Reserved.

using System.Numerics;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
[ConjugateRegistryId(25)]
[ConjugateKey("Unreal.LazyObjectPtr")]
public sealed class TLazyObjectPtr<T> : LazyObjectPtrBase
	, IConjugate<TLazyObjectPtr<T>>
	, ICloneable<TLazyObjectPtr<T>>
	, IEquatable<TLazyObjectPtr<T>>
	, IEqualityOperators<TLazyObjectPtr<T>?, TLazyObjectPtr<T>?, bool>
	, IPersistentObjectWrapper<T>
	where T : UObject
{

	public static TLazyObjectPtr<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);
	
	public static TLazyObjectPtr<T> From<TSource>(TLazyObjectPtr<TSource> other) where TSource : T => new(other);

	public TLazyObjectPtr() => BuildConjugate_Black(UClass.FromType<T>().Unmanaged);
	
	public TLazyObjectPtr(T? target) : this()
	{
		if (target is null)
		{
			return;
		}

		Target = target;
	}
	
	public TLazyObjectPtr<T> Clone() => new(this);
	object ICloneable.Clone() => Clone();

	public bool Equals(TLazyObjectPtr<T>? other) => base.Equals(other);
	public override bool Equals(object? obj) => base.Equals(obj);
	public override int32 GetHashCode() => base.GetHashCode();

	public static implicit operator TLazyObjectPtr<T>(T? target) => new(target);
	public static bool operator ==(TLazyObjectPtr<T>? left, TLazyObjectPtr<T>? right) => Equals(left, right);
	public static bool operator !=(TLazyObjectPtr<T>? left, TLazyObjectPtr<T>? right) => !Equals(left, right);
	
	public static IEqualityComparer<TLazyObjectPtr<T>> DefaultEqualityComparer { get; } = new EqualityComparer();

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

	public bool IsPending
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalIsPending();
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

	private sealed class EqualityComparer : IEqualityComparer<TLazyObjectPtr<T>>
	{
		public bool Equals(TLazyObjectPtr<T>? lhs, TLazyObjectPtr<T>? rhs) => lhs == rhs;
		public int32 GetHashCode(TLazyObjectPtr<T> obj) => obj.GetHashCode();
	}

	private TLazyObjectPtr(IntPtr unmanaged) : base(unmanaged){}
	private TLazyObjectPtr(LazyObjectPtrBase? other) : this() => InternalCopy(other);

	private unsafe void InternalCopy(LazyObjectPtrBase? other)
	{
		if (other is not null)
		{
			LazyObjectPtr_Interop.Copy(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(other));
		}
		else
		{
			Target = null;
		}
	}

	private unsafe T? InternalGet(bool evenIfGarbage)
		=> LazyObjectPtr_Interop.Get(ConjugateHandle.FromConjugate(this), Convert.ToByte(evenIfGarbage)).GetTarget<T>();

	private unsafe void InternalSet(T? target)
		=> LazyObjectPtr_Interop.Set(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(target));

	private unsafe bool InternalIsPending()
		=> LazyObjectPtr_Interop.IsPending(ConjugateHandle.FromConjugate(this)) > 0;
	
}


