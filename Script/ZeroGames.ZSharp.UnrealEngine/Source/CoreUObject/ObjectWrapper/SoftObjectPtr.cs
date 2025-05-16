// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;
using System.Numerics;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
[ConjugateRegistryId(23)]
[ConjugateKey("Unreal.SoftObjectPtr")]
public sealed class TSoftObjectPtr<T> : SoftObjectPtrBase
	, IConjugate<TSoftObjectPtr<T>>
	, ICloneable<TSoftObjectPtr<T>>
	, IEquatable<TSoftObjectPtr<T>>
	, IEqualityOperators<TSoftObjectPtr<T>?, TSoftObjectPtr<T>?, bool>
	, ISoftObjectWrapper<T>
	, IUnrealObjectPath
	where T : UObject
{

	public static TSoftObjectPtr<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);
	
	public static TSoftObjectPtr<T> From<TSource>(TSoftObjectPtr<TSource> other) where TSource : T => new(other);

	public TSoftObjectPtr() => BuildConjugate_Black(UClass.FromType<T>().Unmanaged);
	
	public TSoftObjectPtr(T? target) : this()
	{
		if (target is null)
		{
			return;
		}

		Target = target;
	}
	
	public TSoftObjectPtr<T> Clone() => new(this);
	object ICloneable.Clone() => Clone();

	public bool Equals(TSoftObjectPtr<T>? other) => base.Equals(other);
	public override bool Equals(object? obj) => base.Equals(obj);
	public override int32 GetHashCode() => base.GetHashCode();

	public bool TryLoad([NotNullWhen(true)] out T? target)
	{
		MasterAlcCache.GuardInvariant();
		return InternalTryLoad(out target);
	}
	
	public static implicit operator TSoftObjectPtr<T>(T? target) => new(target);
	public static bool operator ==(TSoftObjectPtr<T>? left, TSoftObjectPtr<T>? right) => Equals(left, right);
	public static bool operator !=(TSoftObjectPtr<T>? left, TSoftObjectPtr<T>? right) => !Equals(left, right);
	
	public static IEqualityComparer<TSoftObjectPtr<T>> DefaultEqualityComparer { get; } = new EqualityComparer();

	public string Path
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return Target?.GetPathName() ?? string.Empty;
		}
	}
	
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
	
	private sealed class EqualityComparer : IEqualityComparer<TSoftObjectPtr<T>>
	{
		public bool Equals(TSoftObjectPtr<T>? lhs, TSoftObjectPtr<T>? rhs) => lhs == rhs;
		public int32 GetHashCode(TSoftObjectPtr<T> obj) => obj.GetHashCode();
	}

	private TSoftObjectPtr(IntPtr unmanaged) : base(unmanaged){}
	private TSoftObjectPtr(SoftObjectPtrBase? other) : this() => InternalCopy(other);

	private unsafe void InternalCopy(SoftObjectPtrBase? other)
	{
		if (other is not null)
		{
			SoftObjectPtr_Interop.Copy(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(other));
		}
		else
		{
			Target = null;
		}
	}

	private unsafe T? InternalGet(bool evenIfGarbage)
		=> SoftObjectPtr_Interop.Get(ConjugateHandle.FromConjugate(this), Convert.ToByte(evenIfGarbage)).GetTarget<T>();

	private unsafe void InternalSet(T? target)
		=> SoftObjectPtr_Interop.Set(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(target));

	private unsafe bool InternalIsPending()
		=> SoftObjectPtr_Interop.IsPending(ConjugateHandle.FromConjugate(this)) > 0;

	private unsafe bool InternalTryLoad([NotNullWhen(true)] out T? target)
	{
		target = SoftObjectPtr_Interop.Load(ConjugateHandle.FromConjugate(this)).GetTarget<T>();
		return target is not null;
	}
	
}


