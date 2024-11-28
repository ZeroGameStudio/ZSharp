// Copyright Zero Games. All Rights Reserved.

using System.Numerics;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
[ConjugateRegistryId(27)]
public sealed class StrongObjectPtr<T> : StrongObjectPtrBase
	, IConjugate<StrongObjectPtr<T>>
	, ICloneable<StrongObjectPtr<T>>
	, IEquatable<StrongObjectPtr<T>>
	, IEqualityOperators<StrongObjectPtr<T>?, StrongObjectPtr<T>?, bool>
	, IUnrealObjectWrapper<T>
	where T : UnrealObject
{

	public static StrongObjectPtr<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);
	
	public static StrongObjectPtr<T> From<TSource>(StrongObjectPtr<TSource> other) where TSource : T => new(other);

	public StrongObjectPtr()
	{
		Unmanaged = MasterAlcCache.Instance.BuildConjugate(this, GetStaticClass(typeof(T)).Unmanaged);
	}
	
	public StrongObjectPtr(T? target) : this()
	{
		if (target is null)
		{
			return;
		}

		Target = target;
	}
	
	public StrongObjectPtr<T> Clone() => new(this);
	object ICloneable.Clone() => Clone();

	public bool Equals(StrongObjectPtr<T>? other) => base.Equals(other);
	public override bool Equals(object? obj) => base.Equals(obj);
	public override int32 GetHashCode() => base.GetHashCode();
	
	public static implicit operator StrongObjectPtr<T>(T? target) => new(target);
	public static bool operator ==(StrongObjectPtr<T>? left, StrongObjectPtr<T>? right) => Equals(left, right);
	public static bool operator !=(StrongObjectPtr<T>? left, StrongObjectPtr<T>? right) => !Equals(left, right);
	
	public static IEqualityComparer<StrongObjectPtr<T>> DefaultEqualityComparer { get; } = new EqualityComparer();

	public T? Target
	{
		get => InternalGet(false);
		set => InternalSet(value);
	}

	public T? TargetEvenIfGarbage => InternalGet(true);

	public bool IsValid => InternalIsValid(false);
	public bool IsValidEventIfGarbage => InternalIsValid(true);
	public bool IsNull => InternalIsNull();

	private sealed class EqualityComparer : IEqualityComparer<StrongObjectPtr<T>>
	{
		public bool Equals(StrongObjectPtr<T>? lhs, StrongObjectPtr<T>? rhs) => lhs == rhs;
		public int32 GetHashCode(StrongObjectPtr<T> obj) => obj.GetHashCode();
	}

	private StrongObjectPtr(IntPtr unmanaged) : base(unmanaged){}
	private StrongObjectPtr(StrongObjectPtrBase? other) : this() => InternalCopy(other);

	private unsafe void InternalCopy(StrongObjectPtrBase? other)
	{
		Thrower.ThrowIfNotInGameThread();
		MasterAlcCache.Instance.GuardUnloaded();
		
		if (other is not null)
		{
			StrongObjectPtr_Interop.Copy(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(other));
		}
		else
		{
			Target = null;
		}
	}

	private unsafe T? InternalGet(bool evenIfGarbage)
	{
		Thrower.ThrowIfNotInGameThread();
		MasterAlcCache.Instance.GuardUnloaded();
		return StrongObjectPtr_Interop.Get(ConjugateHandle.FromConjugate(this), Convert.ToByte(evenIfGarbage)).GetTarget<T>();
	}

	private unsafe void InternalSet(T? target)
	{
		Thrower.ThrowIfNotInGameThread();
		MasterAlcCache.Instance.GuardUnloaded();
		StrongObjectPtr_Interop.Set(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(target));
	}
	
	private unsafe bool InternalIsValid(bool evenIfGarbage)
	{
		Thrower.ThrowIfNotInGameThread();
		MasterAlcCache.Instance.GuardUnloaded();
		return StrongObjectPtr_Interop.IsValid(ConjugateHandle.FromConjugate(this), Convert.ToByte(evenIfGarbage)) > 0;
	}

	private unsafe bool InternalIsNull()
	{
		Thrower.ThrowIfNotInGameThread();
		MasterAlcCache.Instance.GuardUnloaded();
		return StrongObjectPtr_Interop.IsNull(ConjugateHandle.FromConjugate(this)) > 0;
	}
	
}


