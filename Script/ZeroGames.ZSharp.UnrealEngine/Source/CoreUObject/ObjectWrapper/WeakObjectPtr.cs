// Copyright Zero Games. All Rights Reserved.

using System.Numerics;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
[ConjugateRegistryId(24)]
[ConjugateKey("Unreal.WeakObjectPtr")]
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

	public WeakObjectPtr() => BuildConjugate_Black(UnrealClass.FromType<T>().Unmanaged);
	
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
	
	protected override UnrealObject? InternalGetTarget(bool evenIfGarbage) => evenIfGarbage ? TargetEvenIfGarbage : Target;
		
	protected override void InternalSetTarget(UnrealObject? target)
	{
		if (target is not null && target is not T)
		{
			throw new ArgumentOutOfRangeException(nameof(target));
		}

		Target = (T?)target;
	}

	private sealed class EqualityComparer : IEqualityComparer<WeakObjectPtr<T>>
	{
		public bool Equals(WeakObjectPtr<T>? lhs, WeakObjectPtr<T>? rhs) => lhs == rhs;
		public int32 GetHashCode(WeakObjectPtr<T> obj) => obj.GetHashCode();
	}

	private WeakObjectPtr(IntPtr unmanaged) : base(unmanaged){}
	private WeakObjectPtr(WeakObjectPtrBase? other) : this() => InternalCopy(other);

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


