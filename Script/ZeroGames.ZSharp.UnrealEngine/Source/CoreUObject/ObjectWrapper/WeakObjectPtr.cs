// Copyright Zero Games. All Rights Reserved.

using System.Numerics;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
[ConjugateRegistryId(24)]
[ConjugateKey("Unreal.WeakObjectPtr")]
public sealed class TWeakObjectPtr<T> : WeakObjectPtrBase
	, IConjugate<TWeakObjectPtr<T>>
	, ICloneable<TWeakObjectPtr<T>>
	, IEquatable<TWeakObjectPtr<T>>
	, IEqualityOperators<TWeakObjectPtr<T>?, TWeakObjectPtr<T>?, bool>
	, IWeakObjectWrapper<T>
	where T : UObject
{

	public static TWeakObjectPtr<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);
	
	public static TWeakObjectPtr<T> From<TSource>(TWeakObjectPtr<TSource> other) where TSource : T => new(other);

	public TWeakObjectPtr() => BuildConjugate_Black(UClass.FromType<T>().Unmanaged);
	
	public TWeakObjectPtr(T? target) : this()
	{
		if (target is null)
		{
			return;
		}

		Target = target;
	}
	
	public TWeakObjectPtr<T> Clone() => new(this);
	object ICloneable.Clone() => Clone();

	public bool Equals(TWeakObjectPtr<T>? other) => base.Equals(other);
	public override bool Equals(object? obj) => base.Equals(obj);
	public override int32 GetHashCode() => base.GetHashCode();
	
	public static implicit operator TWeakObjectPtr<T>(T? target) => new(target);
	public static bool operator ==(TWeakObjectPtr<T>? left, TWeakObjectPtr<T>? right) => Equals(left, right);
	public static bool operator !=(TWeakObjectPtr<T>? left, TWeakObjectPtr<T>? right) => !Equals(left, right);
	
	public static IEqualityComparer<TWeakObjectPtr<T>> DefaultEqualityComparer { get; } = new EqualityComparer();

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

	public bool IsStale
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalIsStale(true);
		}
	}

	public bool IsStaleExcludingGarbage
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalIsStale(false);
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

	private sealed class EqualityComparer : IEqualityComparer<TWeakObjectPtr<T>>
	{
		public bool Equals(TWeakObjectPtr<T>? lhs, TWeakObjectPtr<T>? rhs) => lhs == rhs;
		public int32 GetHashCode(TWeakObjectPtr<T> obj) => obj.GetHashCode();
	}

	private TWeakObjectPtr(IntPtr unmanaged) : base(unmanaged){}
	private TWeakObjectPtr(WeakObjectPtrBase? other) : this() => InternalCopy(other);

	private unsafe void InternalCopy(WeakObjectPtrBase? other)
	{
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
		=> WeakObjectPtr_Interop.Get(ConjugateHandle.FromConjugate(this), Convert.ToByte(evenIfGarbage)).GetTarget<T>();

	private unsafe void InternalSet(T? target)
		=> WeakObjectPtr_Interop.Set(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(target));

	private unsafe bool InternalIsStale(bool includingGarbage)
		=> WeakObjectPtr_Interop.IsStale(ConjugateHandle.FromConjugate(this), Convert.ToByte(includingGarbage)) > 0;
	
}


