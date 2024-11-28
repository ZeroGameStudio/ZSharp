// Copyright Zero Games. All Rights Reserved.

using System.Numerics;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public abstract class SubclassOfBase : PlainExportedObjectBase
	, IEquatable<SubclassOfBase>
	, IEqualityOperators<SubclassOfBase?, SubclassOfBase?, bool>
{
	
	public bool Equals(SubclassOfBase? other) => ReferenceEquals(this, other) || InternalEquals(other);
	public override bool Equals(object? obj) => obj is SubclassOfBase other && Equals(other);
	public override int32 GetHashCode() => InternalGetHashCode();
	
	public static bool operator ==(SubclassOfBase? left, SubclassOfBase? right) => Equals(left, right);
	public static bool operator !=(SubclassOfBase? left, SubclassOfBase? right) => !Equals(left, right);
	
	protected SubclassOfBase(){}
	protected SubclassOfBase(IntPtr unmanaged) : base(unmanaged){}

	private unsafe bool InternalEquals(SubclassOfBase? other)
	{
		Thrower.ThrowIfNotInGameThread();
		return other is not null && SubclassOf_Interop.Identical(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(other)) > 0;
	}
	
	private unsafe int32 InternalGetHashCode()
	{
		Thrower.ThrowIfNotInGameThread();
		return SubclassOf_Interop.Hash(ConjugateHandle.FromConjugate(this));
	}
	
}