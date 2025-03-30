// Copyright Zero Games. All Rights Reserved.

using System.Numerics;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public abstract class LazyObjectPtrBase : UnrealConjugateBase
	, IEquatable<LazyObjectPtrBase>
	, IEqualityOperators<LazyObjectPtrBase?, LazyObjectPtrBase?, bool>
{
	
	public bool Equals(LazyObjectPtrBase? other)
	{
		MasterAlcCache.GuardInvariant();
		return ReferenceEquals(this, other) || InternalEquals(other);
	}
	public override bool Equals(object? obj) => obj is LazyObjectPtrBase other && Equals(other);
	
	public override int32 GetHashCode()
	{
		MasterAlcCache.GuardInvariant();
		return InternalGetHashCode();
	}
	
	public static bool operator ==(LazyObjectPtrBase? left, LazyObjectPtrBase? right) => Equals(left, right);
	public static bool operator !=(LazyObjectPtrBase? left, LazyObjectPtrBase? right) => !Equals(left, right);
	
	protected LazyObjectPtrBase(){}
	protected LazyObjectPtrBase(IntPtr unmanaged) : base(unmanaged){}

	private unsafe bool InternalEquals(LazyObjectPtrBase? other)
		=> other is not null && LazyObjectPtr_Interop.Identical(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(other)) > 0;
	
	private unsafe int32 InternalGetHashCode()
		=> LazyObjectPtr_Interop.Hash(ConjugateHandle.FromConjugate(this));
	
}