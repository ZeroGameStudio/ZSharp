// Copyright Zero Games. All Rights Reserved.

using System.Numerics;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public abstract class SoftObjectPtrBase : UnrealConjugateBase
	, IEquatable<SoftObjectPtrBase>
	, IEqualityOperators<SoftObjectPtrBase?, SoftObjectPtrBase?, bool>
{
	
	public bool Equals(SoftObjectPtrBase? other)
	{
		MasterAlcCache.GuardInvariant();
		return ReferenceEquals(this, other) || InternalEquals(other);
	}
	public override bool Equals(object? obj) => obj is SoftObjectPtrBase other && Equals(other);
	
	public override int32 GetHashCode()
	{
		MasterAlcCache.GuardInvariant();
		return InternalGetHashCode();
	}
	
	public static bool operator ==(SoftObjectPtrBase? left, SoftObjectPtrBase? right) => Equals(left, right);
	public static bool operator !=(SoftObjectPtrBase? left, SoftObjectPtrBase? right) => !Equals(left, right);
	
	protected SoftObjectPtrBase(){}
	protected SoftObjectPtrBase(IntPtr unmanaged) : base(unmanaged){}

	private unsafe bool InternalEquals(SoftObjectPtrBase? other)
		=> other is not null && SoftObjectPtr_Interop.Identical(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(other)) > 0;
	
	private unsafe int32 InternalGetHashCode()
		=> SoftObjectPtr_Interop.Hash(ConjugateHandle.FromConjugate(this));
	
}