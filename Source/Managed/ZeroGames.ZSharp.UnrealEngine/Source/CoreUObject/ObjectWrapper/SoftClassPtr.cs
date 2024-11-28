// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;
using System.Numerics;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
[ConjugateRegistryId(22)]
public sealed class SoftClassPtr<T> : SoftClassPtrBase
	, IConjugate<SoftClassPtr<T>>
	, ICloneable<SoftClassPtr<T>>
	, IEquatable<SoftClassPtr<T>>
	, IEqualityOperators<SoftClassPtr<T>?, SoftClassPtr<T>?, bool>
	, ISoftObjectWrapper<UnrealClass>
	where T : class, IUnrealObject
{

	public static SoftClassPtr<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);
	
	public static SoftClassPtr<T> From<TSource>(SoftClassPtr<TSource> other) where TSource : class, T => new(other);

	public SoftClassPtr()
	{
		Unmanaged = MasterAlcCache.Instance.BuildConjugate(this, GetStaticClass(typeof(T)).Unmanaged);
	}
	
	public SoftClassPtr(UnrealClass? target) : this()
	{
		if (target is null)
		{
			return;
		}

		Target = target;
	}
	
	public SoftClassPtr<T> Clone() => new(this);
	object ICloneable.Clone() => Clone();

	public bool Equals(SoftClassPtr<T>? other) => base.Equals(other);
	public override bool Equals(object? obj) => base.Equals(obj);
	public override int32 GetHashCode() => base.GetHashCode();
	
	public bool TryLoad([NotNullWhen(true)] out UnrealClass? target) => InternalTryLoad(out target);
	
	public static implicit operator SoftClassPtr<T>(UnrealClass? target) => new(target);
	public static bool operator ==(SoftClassPtr<T>? left, SoftClassPtr<T>? right) => Equals(left, right);
	public static bool operator !=(SoftClassPtr<T>? left, SoftClassPtr<T>? right) => !Equals(left, right);
	
	public static IEqualityComparer<SoftClassPtr<T>> DefaultEqualityComparer { get; } = new EqualityComparer();

	public UnrealClass? Target
	{
		get => InternalGet(false);
		set => InternalSet(value);
	}

	public UnrealClass? TargetEvenIfGarbage => InternalGet(true);

	public bool IsValid => InternalIsValid(false);
	public bool IsValidEventIfGarbage => InternalIsValid(true);
	public bool IsNull => InternalIsNull();
	public bool IsPending => InternalIsPending();

	private sealed class EqualityComparer : IEqualityComparer<SoftClassPtr<T>>
	{
		public bool Equals(SoftClassPtr<T>? lhs, SoftClassPtr<T>? rhs) => lhs == rhs;
		public int32 GetHashCode(SoftClassPtr<T> obj) => obj.GetHashCode();
	}

	private SoftClassPtr(IntPtr unmanaged) : base(unmanaged){}
	private SoftClassPtr(SoftClassPtrBase? other) : this() => InternalCopy(other);

	private unsafe void InternalCopy(SoftClassPtrBase? other)
	{
		Thrower.ThrowIfNotInGameThread();
		MasterAlcCache.Instance.GuardUnloaded();
		
		if (other is not null)
		{
			SoftClassPtr_Interop.Copy(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(other));
		}
		else
		{
			Target = null;
		}
	}

	private unsafe UnrealClass? InternalGet(bool evenIfGarbage)
	{
		Thrower.ThrowIfNotInGameThread();
		MasterAlcCache.Instance.GuardUnloaded();
		return SoftClassPtr_Interop.Get(ConjugateHandle.FromConjugate(this), Convert.ToByte(evenIfGarbage)).GetTarget<UnrealClass>();
	}

	private unsafe void InternalSet(UnrealClass? target)
	{
		Thrower.ThrowIfNotInGameThread();
		MasterAlcCache.Instance.GuardUnloaded();
		if (target is not null && !target.IsChildOf<T>())
		{
			throw new ArgumentOutOfRangeException($"Target type {target.GetType().FullName} mismatch.");
		}
		
		SoftClassPtr_Interop.Set(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(target));
	}
	
	private unsafe bool InternalIsValid(bool evenIfGarbage)
	{
		Thrower.ThrowIfNotInGameThread();
		MasterAlcCache.Instance.GuardUnloaded();
		return SoftClassPtr_Interop.IsValid(ConjugateHandle.FromConjugate(this), Convert.ToByte(evenIfGarbage)) > 0;
	}

	private unsafe bool InternalIsNull()
	{
		Thrower.ThrowIfNotInGameThread();
		MasterAlcCache.Instance.GuardUnloaded();
		return SoftClassPtr_Interop.IsNull(ConjugateHandle.FromConjugate(this)) > 0;
	}
	
	private unsafe bool InternalIsPending()
	{
		Thrower.ThrowIfNotInGameThread();
		MasterAlcCache.Instance.GuardUnloaded();
		return SoftClassPtr_Interop.IsPending(ConjugateHandle.FromConjugate(this)) > 0;
	}

	private unsafe bool InternalTryLoad([NotNullWhen(true)] out UnrealClass? target)
	{
		Thrower.ThrowIfNotInGameThread();
		MasterAlcCache.Instance.GuardUnloaded();
		target = SoftClassPtr_Interop.Load(ConjugateHandle.FromConjugate(this)).GetTarget<UnrealClass>();
		return target is not null;
	}
	
}


