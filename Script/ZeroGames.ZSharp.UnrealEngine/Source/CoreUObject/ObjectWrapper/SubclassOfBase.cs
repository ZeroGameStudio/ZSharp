// Copyright Zero Games. All Rights Reserved.

using System.Numerics;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public abstract class SubclassOfBase : UnrealObjectWrapperBase
	, IEquatable<SubclassOfBase>
	, IEqualityOperators<SubclassOfBase?, SubclassOfBase?, bool>
{
	
	public bool Equals(SubclassOfBase? other)
	{
		MasterAlcCache.GuardInvariant();
		return ReferenceEquals(this, other) || InternalEquals(other);
	}
	public override bool Equals(object? obj) => obj is SubclassOfBase other && Equals(other);
	
	public override int32 GetHashCode()
	{
		MasterAlcCache.GuardInvariant();
		return InternalGetHashCode();
	}
	
	public static bool operator ==(SubclassOfBase? left, SubclassOfBase? right) => Equals(left, right);
	public static bool operator !=(SubclassOfBase? left, SubclassOfBase? right) => !Equals(left, right);
	
	protected SubclassOfBase(){}
	protected SubclassOfBase(IntPtr unmanaged) : base(unmanaged){}
	
	protected override unsafe bool InternalIsValid(bool evenIfGarbage)
		=> SubclassOf_Interop.IsValid(ConjugateHandle.FromConjugate(this), Convert.ToByte(evenIfGarbage)) > 0;

	protected override unsafe bool InternalIsNull()
		=> SubclassOf_Interop.IsNull(ConjugateHandle.FromConjugate(this)) > 0;

	private unsafe bool InternalEquals(SubclassOfBase? other)
		=> other is not null && SubclassOf_Interop.Identical(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(other)) > 0;
	
	private unsafe int32 InternalGetHashCode()
		=> SubclassOf_Interop.Hash(ConjugateHandle.FromConjugate(this));
	
}