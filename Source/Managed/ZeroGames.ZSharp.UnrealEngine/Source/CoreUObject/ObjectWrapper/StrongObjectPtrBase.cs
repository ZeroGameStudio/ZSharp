// Copyright Zero Games. All Rights Reserved.

using System.Numerics;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public abstract class StrongObjectPtrBase : PlainExportedObjectBase
	, IEquatable<StrongObjectPtrBase>
	, IEqualityOperators<StrongObjectPtrBase?, StrongObjectPtrBase?, bool>
{
	
	public bool Equals(StrongObjectPtrBase? other) => ReferenceEquals(this, other) || InternalEquals(other);
	public override bool Equals(object? obj) => obj is StrongObjectPtrBase other && Equals(other);
	public override int32 GetHashCode() => InternalGetHashCode();
	
	public static bool operator ==(StrongObjectPtrBase? left, StrongObjectPtrBase? right) => Equals(left, right);
	public static bool operator !=(StrongObjectPtrBase? left, StrongObjectPtrBase? right) => !Equals(left, right);
	
	protected StrongObjectPtrBase(){}
	protected StrongObjectPtrBase(IntPtr unmanaged) : base(unmanaged){}

	private unsafe bool InternalEquals(StrongObjectPtrBase? other)
	{
		Thrower.ThrowIfNotInGameThread();
		MasterAlcCache.Instance.GuardUnloaded();
		return other is not null && StrongObjectPtr_Interop.Identical(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(other)) > 0;
	}
	
	private unsafe int32 InternalGetHashCode()
	{
		Thrower.ThrowIfNotInGameThread();
		MasterAlcCache.Instance.GuardUnloaded();
		return StrongObjectPtr_Interop.Hash(ConjugateHandle.FromConjugate(this));
	}
	
}