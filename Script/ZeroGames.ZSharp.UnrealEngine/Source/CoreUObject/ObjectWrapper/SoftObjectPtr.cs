// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;
using System.Numerics;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
[ConjugateRegistryId(23)]
[ConjugateKey("Unreal.SoftObjectPtr")]
public sealed class SoftObjectPtr<T> : SoftObjectPtrBase
	, IConjugate<SoftObjectPtr<T>>
	, ICloneable<SoftObjectPtr<T>>
	, IEquatable<SoftObjectPtr<T>>
	, IEqualityOperators<SoftObjectPtr<T>?, SoftObjectPtr<T>?, bool>
	, ISoftObjectWrapper<T>
	, IUnrealObjectPath
	where T : UnrealObject
{

	public static SoftObjectPtr<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);
	
	public static SoftObjectPtr<T> From<TSource>(SoftObjectPtr<TSource> other) where TSource : T => new(other);

	public SoftObjectPtr() => BuildConjugate_Black(UnrealClass.FromType<T>().Unmanaged);
	
	public SoftObjectPtr(T? target) : this()
	{
		if (target is null)
		{
			return;
		}

		Target = target;
	}
	
	public SoftObjectPtr<T> Clone() => new(this);
	object ICloneable.Clone() => Clone();

	public bool Equals(SoftObjectPtr<T>? other) => base.Equals(other);
	public override bool Equals(object? obj) => base.Equals(obj);
	public override int32 GetHashCode() => base.GetHashCode();

	public bool TryLoad([NotNullWhen(true)] out T? target)
	{
		MasterAlcCache.GuardInvariant();
		return InternalTryLoad(out target);
	}
	
	public static implicit operator SoftObjectPtr<T>(T? target) => new(target);
	public static bool operator ==(SoftObjectPtr<T>? left, SoftObjectPtr<T>? right) => Equals(left, right);
	public static bool operator !=(SoftObjectPtr<T>? left, SoftObjectPtr<T>? right) => !Equals(left, right);
	
	public static IEqualityComparer<SoftObjectPtr<T>> DefaultEqualityComparer { get; } = new EqualityComparer();

	public string Path
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return Target?.GetPathName() ?? string.Empty;
		}
	}
	
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

	private sealed class EqualityComparer : IEqualityComparer<SoftObjectPtr<T>>
	{
		public bool Equals(SoftObjectPtr<T>? lhs, SoftObjectPtr<T>? rhs) => lhs == rhs;
		public int32 GetHashCode(SoftObjectPtr<T> obj) => obj.GetHashCode();
	}

	private SoftObjectPtr(IntPtr unmanaged) : base(unmanaged){}
	private SoftObjectPtr(SoftObjectPtrBase? other) : this() => InternalCopy(other);

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
	
	private unsafe bool InternalIsValid(bool evenIfGarbage)
		=> SoftObjectPtr_Interop.IsValid(ConjugateHandle.FromConjugate(this), Convert.ToByte(evenIfGarbage)) > 0;

	private unsafe bool InternalIsNull()
		=> SoftObjectPtr_Interop.IsNull(ConjugateHandle.FromConjugate(this)) > 0;

	private unsafe bool InternalIsPending()
		=> SoftObjectPtr_Interop.IsPending(ConjugateHandle.FromConjugate(this)) > 0;

	private unsafe bool InternalTryLoad([NotNullWhen(true)] out T? target)
	{
		target = SoftObjectPtr_Interop.Load(ConjugateHandle.FromConjugate(this)).GetTarget<T>();
		return target is not null;
	}
	
}


