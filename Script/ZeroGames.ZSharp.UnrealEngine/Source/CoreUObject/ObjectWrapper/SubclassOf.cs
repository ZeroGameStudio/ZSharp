// Copyright Zero Games. All Rights Reserved.

using System.Numerics;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
[ConjugateRegistryId(21)]
[ConjugateKey("Unreal.SubclassOf")]
public sealed class SubclassOf<T> : SubclassOfBase
	, IConjugate<SubclassOf<T>>
	, ICloneable<SubclassOf<T>>
	, IEquatable<SubclassOf<T>>
	, IEqualityOperators<SubclassOf<T>?, SubclassOf<T>?, bool>
	, IUnrealObjectWrapper<UnrealClass>
	where T : class, IUnrealObject
{

	public static SubclassOf<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);
	
	public static SubclassOf<T> From<TSource>(SubclassOf<TSource> other) where TSource : class, T => new(other);

	public SubclassOf() => BuildConjugate_Black(UnrealClass.FromType<T>().Unmanaged);
	
	public SubclassOf(UnrealClass? target) : this()
	{
		if (target is null)
		{
			return;
		}

		Target = target;
	}
	
	public SubclassOf<T> Clone() => new(this);
	object ICloneable.Clone() => Clone();

	public bool Equals(SubclassOf<T>? other) => base.Equals(other);
	public override bool Equals(object? obj) => base.Equals(obj);
	public override int32 GetHashCode() => base.GetHashCode();
	
	public static implicit operator SubclassOf<T>(UnrealClass? target) => new(target);
	public static bool operator ==(SubclassOf<T>? left, SubclassOf<T>? right) => Equals(left, right);
	public static bool operator !=(SubclassOf<T>? left, SubclassOf<T>? right) => !Equals(left, right);
	
	public static IEqualityComparer<SubclassOf<T>> DefaultEqualityComparer { get; } = new EqualityComparer();

	public UnrealClass? Target
	{
		get => InternalGet(false);
		set => InternalSet(value);
	}

	public UnrealClass? TargetEvenIfGarbage => InternalGet(true);

	public bool IsValid => InternalIsValid(false);
	public bool IsValidEventIfGarbage => InternalIsValid(true);
	public bool IsNull => InternalIsNull();

	private sealed class EqualityComparer : IEqualityComparer<SubclassOf<T>>
	{
		public bool Equals(SubclassOf<T>? lhs, SubclassOf<T>? rhs) => lhs == rhs;
		public int32 GetHashCode(SubclassOf<T> obj) => obj.GetHashCode();
	}

	private SubclassOf(IntPtr unmanaged) : base(unmanaged){}
	private SubclassOf(SubclassOfBase? other) : this() => InternalCopy(other);

	private unsafe void InternalCopy(SubclassOfBase? other)
	{
		Thrower.ThrowIfNotInGameThread();
		MasterAlcCache.Instance.GuardUnloaded();
		
		if (other is not null)
		{
			SubclassOf_Interop.Copy(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(other));
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
		return SubclassOf_Interop.Get(ConjugateHandle.FromConjugate(this), Convert.ToByte(evenIfGarbage)).GetTarget<UnrealClass>();
	}

	private unsafe void InternalSet(UnrealClass? target)
	{
		Thrower.ThrowIfNotInGameThread();
		MasterAlcCache.Instance.GuardUnloaded();
		if (target is not null && !target.IsChildOf<T>())
		{
			throw new ArgumentOutOfRangeException($"Target type {target.GetType().FullName} mismatch.");
		}
		
		SubclassOf_Interop.Set(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(target));
	}
	
	private unsafe bool InternalIsValid(bool evenIfGarbage)
	{
		Thrower.ThrowIfNotInGameThread();
		MasterAlcCache.Instance.GuardUnloaded();
		return SubclassOf_Interop.IsValid(ConjugateHandle.FromConjugate(this), Convert.ToByte(evenIfGarbage)) > 0;
	}

	private unsafe bool InternalIsNull()
	{
		Thrower.ThrowIfNotInGameThread();
		MasterAlcCache.Instance.GuardUnloaded();
		return SubclassOf_Interop.IsNull(ConjugateHandle.FromConjugate(this)) > 0;
	}
	
}


