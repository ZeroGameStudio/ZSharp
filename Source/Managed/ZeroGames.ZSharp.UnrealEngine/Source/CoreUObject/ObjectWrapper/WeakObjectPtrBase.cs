// Copyright Zero Games. All Rights Reserved.

using System.Numerics;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

[ConjugateRegistryId(24)]
public abstract class WeakObjectPtrBase : PlainExportedObjectBase
	, IEquatable<WeakObjectPtrBase>
	, IEqualityOperators<WeakObjectPtrBase, WeakObjectPtrBase, bool>
{
	
	public bool Equals(WeakObjectPtrBase? other) => ReferenceEquals(this, other) || InternalEquals(other);
	public override bool Equals(object? obj) => obj is WeakObjectPtrBase other && Equals(other);
	public override int32 GetHashCode() => InternalGetHashCode();
	
	public static bool operator ==(WeakObjectPtrBase? left, WeakObjectPtrBase? right) => Equals(left, right);
	public static bool operator !=(WeakObjectPtrBase? left, WeakObjectPtrBase? right) => !Equals(left, right);
	
	public bool IsValid => InternalIsValid(false);
	public bool IsValidEventIfGarbage => InternalIsValid(true);
	public bool IsNull => InternalIsNull();
	public bool IsStale => InternalIsStale(true);
	public bool IsStaleExcludingGarbage => InternalIsStale(false);
	
	protected WeakObjectPtrBase() {}
	protected WeakObjectPtrBase(IntPtr unmanaged) : base(unmanaged){}

	private unsafe bool InternalEquals(WeakObjectPtrBase? other)
	{
		Thrower.ThrowIfNotInGameThread();
		return other is not null && WeakObjectPtr_Interop.Identical(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(other)) > 0;
	}
	
	private unsafe int32 InternalGetHashCode()
	{
		Thrower.ThrowIfNotInGameThread();
		return WeakObjectPtr_Interop.Hash(ConjugateHandle.FromConjugate(this));
	}
	
	private unsafe bool InternalIsValid(bool evenIfGarbage)
	{
		Thrower.ThrowIfNotInGameThread();
		return WeakObjectPtr_Interop.IsValid(ConjugateHandle.FromConjugate(this), Convert.ToByte(evenIfGarbage)) > 0;
	}

	private unsafe bool InternalIsNull()
	{
		Thrower.ThrowIfNotInGameThread();
		return WeakObjectPtr_Interop.IsNull(ConjugateHandle.FromConjugate(this)) > 0;
	}

	private unsafe bool InternalIsStale(bool includingGarbage)
	{
		Thrower.ThrowIfNotInGameThread();
		return WeakObjectPtr_Interop.IsStale(ConjugateHandle.FromConjugate(this), Convert.ToByte(includingGarbage)) > 0;
	}
	
}


