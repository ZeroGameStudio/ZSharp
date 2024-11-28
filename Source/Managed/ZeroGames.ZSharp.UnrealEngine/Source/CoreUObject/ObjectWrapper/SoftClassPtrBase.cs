// Copyright Zero Games. All Rights Reserved.

using System.Numerics;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public abstract class SoftClassPtrBase : PlainExportedObjectBase
	, IEquatable<SoftClassPtrBase>
	, IEqualityOperators<SoftClassPtrBase?, SoftClassPtrBase?, bool>
{
	
	public bool Equals(SoftClassPtrBase? other) => ReferenceEquals(this, other) || InternalEquals(other);
	public override bool Equals(object? obj) => obj is SoftClassPtrBase other && Equals(other);
	public override int32 GetHashCode() => InternalGetHashCode();
	
	public static bool operator ==(SoftClassPtrBase? left, SoftClassPtrBase? right) => Equals(left, right);
	public static bool operator !=(SoftClassPtrBase? left, SoftClassPtrBase? right) => !Equals(left, right);
	
	protected SoftClassPtrBase(){}
	protected SoftClassPtrBase(IntPtr unmanaged) : base(unmanaged){}

	private unsafe bool InternalEquals(SoftClassPtrBase? other)
	{
		Thrower.ThrowIfNotInGameThread();
		return other is not null && SoftClassPtr_Interop.Identical(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(other)) > 0;
	}
	
	private unsafe int32 InternalGetHashCode()
	{
		Thrower.ThrowIfNotInGameThread();
		return SoftClassPtr_Interop.Hash(ConjugateHandle.FromConjugate(this));
	}
	
}