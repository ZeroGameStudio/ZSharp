// Copyright Zero Games. All Rights Reserved.

using System.Numerics;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
[ConjugateRegistryId(21)]
[ConjugateKey("Unreal.SubclassOf")]
public sealed class TSubclassOf<T> : SubclassOfBase
	, IConjugate<TSubclassOf<T>>
	, ICloneable<TSubclassOf<T>>
	, IEquatable<TSubclassOf<T>>
	, IEqualityOperators<TSubclassOf<T>?, TSubclassOf<T>?, bool>
	, IUnrealObjectWrapper<UClass>
	where T : class, IUnrealObject
{

	public static TSubclassOf<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);
	
	public static TSubclassOf<T> From<TSource>(TSubclassOf<TSource> other) where TSource : class, T => new(other);

	public TSubclassOf() => BuildConjugate_Black(UClass.FromType<T>().Unmanaged);
	
	public TSubclassOf(UClass? target) : this()
	{
		if (target is null)
		{
			return;
		}

		Target = target;
	}
	
	public TSubclassOf<T> Clone() => new(this);
	object ICloneable.Clone() => Clone();

	public bool Equals(TSubclassOf<T>? other) => base.Equals(other);
	public override bool Equals(object? obj) => base.Equals(obj);
	public override int32 GetHashCode() => base.GetHashCode();
	
	public static implicit operator TSubclassOf<T>(UClass? target) => new(target);
	public static bool operator ==(TSubclassOf<T>? left, TSubclassOf<T>? right) => Equals(left, right);
	public static bool operator !=(TSubclassOf<T>? left, TSubclassOf<T>? right) => !Equals(left, right);
	
	public static IEqualityComparer<TSubclassOf<T>> DefaultEqualityComparer { get; } = new EqualityComparer();

	public new UClass? Target
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

	public new UClass? TargetEvenIfGarbage
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
		if (target is not null && target is not UClass)
		{
			throw new ArgumentOutOfRangeException(nameof(target));
		}

		Target = (UClass?)target;
	}

	private sealed class EqualityComparer : IEqualityComparer<TSubclassOf<T>>
	{
		public bool Equals(TSubclassOf<T>? lhs, TSubclassOf<T>? rhs) => lhs == rhs;
		public int32 GetHashCode(TSubclassOf<T> obj) => obj.GetHashCode();
	}

	private TSubclassOf(IntPtr unmanaged) : base(unmanaged){}
	private TSubclassOf(SubclassOfBase? other) : this() => InternalCopy(other);

	private unsafe void InternalCopy(SubclassOfBase? other)
	{
		if (other is not null)
		{
			SubclassOf_Interop.Copy(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(other));
		}
		else
		{
			Target = null;
		}
	}

	private unsafe UClass? InternalGet(bool evenIfGarbage)
		=> SubclassOf_Interop.Get(ConjugateHandle.FromConjugate(this), Convert.ToByte(evenIfGarbage)).GetTarget<UClass>();

	private unsafe void InternalSet(UClass? target)
	{
		if (target is not null && !target.IsChildOf<T>())
		{
			throw new ArgumentOutOfRangeException($"Target type {target.GetType().FullName} mismatch.");
		}
		
		SubclassOf_Interop.Set(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(target));
	}
	
}


