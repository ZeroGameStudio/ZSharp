// Copyright Zero Games. All Rights Reserved.

using System.Numerics;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
[ConjugateRegistryId(25)]
[ConjugateKey("Unreal.LazyObjectPtr")]
public sealed class LazyObjectPtr<T> : LazyObjectPtrBase
	, IConjugate<LazyObjectPtr<T>>
	, ICloneable<LazyObjectPtr<T>>
	, IEquatable<LazyObjectPtr<T>>
	, IEqualityOperators<LazyObjectPtr<T>?, LazyObjectPtr<T>?, bool>
	, IPersistentObjectWrapper<T>
	where T : UnrealObject
{

	public static LazyObjectPtr<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);
	
	public static LazyObjectPtr<T> From<TSource>(LazyObjectPtr<TSource> other) where TSource : T => new(other);

	public LazyObjectPtr() => BuildConjugate_Black(UnrealClass.FromType<T>().Unmanaged);
	
	public LazyObjectPtr(T? target) : this()
	{
		if (target is null)
		{
			return;
		}

		Target = target;
	}
	
	public LazyObjectPtr<T> Clone() => new(this);
	object ICloneable.Clone() => Clone();

	public bool Equals(LazyObjectPtr<T>? other) => base.Equals(other);
	public override bool Equals(object? obj) => base.Equals(obj);
	public override int32 GetHashCode() => base.GetHashCode();

	public static implicit operator LazyObjectPtr<T>(T? target) => new(target);
	public static bool operator ==(LazyObjectPtr<T>? left, LazyObjectPtr<T>? right) => Equals(left, right);
	public static bool operator !=(LazyObjectPtr<T>? left, LazyObjectPtr<T>? right) => !Equals(left, right);
	
	public static IEqualityComparer<LazyObjectPtr<T>> DefaultEqualityComparer { get; } = new EqualityComparer();

	public T? Target
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

	public T? TargetEvenIfGarbage
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalGet(true);
		}
	}

	public bool IsValid
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalIsValid(false);
		}
	}

	public bool IsValidEventIfGarbage
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalIsValid(true);
		}
	}

	public bool IsNull
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalIsNull();
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

	private sealed class EqualityComparer : IEqualityComparer<LazyObjectPtr<T>>
	{
		public bool Equals(LazyObjectPtr<T>? lhs, LazyObjectPtr<T>? rhs) => lhs == rhs;
		public int32 GetHashCode(LazyObjectPtr<T> obj) => obj.GetHashCode();
	}

	private LazyObjectPtr(IntPtr unmanaged) : base(unmanaged){}
	private LazyObjectPtr(LazyObjectPtrBase? other) : this() => InternalCopy(other);

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
	
	private unsafe bool InternalIsValid(bool evenIfGarbage)
		=> LazyObjectPtr_Interop.IsValid(ConjugateHandle.FromConjugate(this), Convert.ToByte(evenIfGarbage)) > 0;

	private unsafe bool InternalIsNull()
		=> LazyObjectPtr_Interop.IsNull(ConjugateHandle.FromConjugate(this)) > 0;

	private unsafe bool InternalIsPending()
		=> LazyObjectPtr_Interop.IsPending(ConjugateHandle.FromConjugate(this)) > 0;
	
}


