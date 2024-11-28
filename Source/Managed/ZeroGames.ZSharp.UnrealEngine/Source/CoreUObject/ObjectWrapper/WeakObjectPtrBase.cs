// Copyright Zero Games. All Rights Reserved.

using System.Numerics;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public abstract class WeakObjectPtrBase : PlainExportedObjectBase
	, IEquatable<WeakObjectPtrBase>
	, IEqualityOperators<WeakObjectPtrBase?, WeakObjectPtrBase?, bool>
{
	
	public bool Equals(WeakObjectPtrBase? other) => ReferenceEquals(this, other) || InternalEquals(other);
	public override bool Equals(object? obj) => obj is WeakObjectPtrBase other && Equals(other);
	public override int32 GetHashCode() => InternalGetHashCode();
	
	public static bool operator ==(WeakObjectPtrBase? left, WeakObjectPtrBase? right) => Equals(left, right);
	public static bool operator !=(WeakObjectPtrBase? left, WeakObjectPtrBase? right) => !Equals(left, right);
	
	protected WeakObjectPtrBase(){}
	protected WeakObjectPtrBase(IntPtr unmanaged) : base(unmanaged){}

	private unsafe bool InternalEquals(WeakObjectPtrBase? other)
	{
		Thrower.ThrowIfNotInGameThread();
		MasterAlcCache.Instance.GuardUnloaded();
		return other is not null && WeakObjectPtr_Interop.Identical(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(other)) > 0;
	}
	
	private unsafe int32 InternalGetHashCode()
	{
		Thrower.ThrowIfNotInGameThread();
		MasterAlcCache.Instance.GuardUnloaded();
		return WeakObjectPtr_Interop.Hash(ConjugateHandle.FromConjugate(this));
	}
	
}


