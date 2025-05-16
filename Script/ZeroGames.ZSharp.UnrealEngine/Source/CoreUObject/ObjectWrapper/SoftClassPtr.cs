// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;
using System.Numerics;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
[ConjugateRegistryId(22)]
[ConjugateKey("Unreal.SoftClassPtr")]
public sealed class TSoftClassPtr<T> : SoftClassPtrBase
	, IConjugate<TSoftClassPtr<T>>
	, ICloneable<TSoftClassPtr<T>>
	, IEquatable<TSoftClassPtr<T>>
	, IEqualityOperators<TSoftClassPtr<T>?, TSoftClassPtr<T>?, bool>
	, ISoftObjectWrapper<UClass>
	, IUnrealClassPath
	where T : class, IUnrealObject
{

	public static TSoftClassPtr<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);
	
	public static TSoftClassPtr<T> From<TSource>(TSoftClassPtr<TSource> other) where TSource : class, T => new(other);

	public TSoftClassPtr() => BuildConjugate_Black(UClass.FromType<T>().Unmanaged);
	
	public TSoftClassPtr(UClass? target) : this()
	{
		if (target is null)
		{
			return;
		}

		Target = target;
	}
	
	public TSoftClassPtr<T> Clone() => new(this);
	object ICloneable.Clone() => Clone();

	public bool Equals(TSoftClassPtr<T>? other) => base.Equals(other);
	public override bool Equals(object? obj) => base.Equals(obj);
	public override int32 GetHashCode() => base.GetHashCode();
	
	public bool TryLoad([NotNullWhen(true)] out UClass? target)
	{
		MasterAlcCache.GuardInvariant();
		return InternalTryLoad(out target);
	}
	
	public static implicit operator TSoftClassPtr<T>(UClass? target) => new(target);
	public static bool operator ==(TSoftClassPtr<T>? left, TSoftClassPtr<T>? right) => Equals(left, right);
	public static bool operator !=(TSoftClassPtr<T>? left, TSoftClassPtr<T>? right) => !Equals(left, right);
	
	public static IEqualityComparer<TSoftClassPtr<T>> DefaultEqualityComparer { get; } = new EqualityComparer();

	public string Path
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return Target?.GetPathName() ?? string.Empty;
		}
	}
	
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
		if (target is not null && target is not UClass)
		{
			throw new ArgumentOutOfRangeException(nameof(target));
		}

		Target = (UClass?)target;
	}
	
	private sealed class EqualityComparer : IEqualityComparer<TSoftClassPtr<T>>
	{
		public bool Equals(TSoftClassPtr<T>? lhs, TSoftClassPtr<T>? rhs) => lhs == rhs;
		public int32 GetHashCode(TSoftClassPtr<T> obj) => obj.GetHashCode();
	}

	private TSoftClassPtr(IntPtr unmanaged) : base(unmanaged){}
	private TSoftClassPtr(SoftClassPtrBase? other) : this() => InternalCopy(other);

	private unsafe void InternalCopy(SoftClassPtrBase? other)
	{
		if (other is not null)
		{
			SoftClassPtr_Interop.Copy(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(other));
		}
		else
		{
			Target = null;
		}
	}

	private unsafe UClass? InternalGet(bool evenIfGarbage)
		=> SoftClassPtr_Interop.Get(ConjugateHandle.FromConjugate(this), Convert.ToByte(evenIfGarbage)).GetTarget<UClass>();

	private unsafe void InternalSet(UClass? target)
	{
		if (target is not null && !target.IsChildOf<T>())
		{
			throw new ArgumentOutOfRangeException($"Target type {target.GetType().FullName} mismatch.");
		}
		
		SoftClassPtr_Interop.Set(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(target));
	}

	private unsafe bool InternalIsPending()
		=> SoftClassPtr_Interop.IsPending(ConjugateHandle.FromConjugate(this)) > 0;

	private unsafe bool InternalTryLoad([NotNullWhen(true)] out UClass? target)
	{
		target = SoftClassPtr_Interop.Load(ConjugateHandle.FromConjugate(this)).GetTarget<UClass>();
		return target is not null;
	}
	
}


