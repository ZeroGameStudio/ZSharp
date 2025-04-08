// Copyright Zero Games. All Rights Reserved.

using System.Numerics;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public abstract class WeakObjectPtrBase : UnrealObjectWrapperBase
	, IEquatable<WeakObjectPtrBase>
	, IEqualityOperators<WeakObjectPtrBase?, WeakObjectPtrBase?, bool>
{
	
	public bool Equals(WeakObjectPtrBase? other)
	{
		MasterAlcCache.GuardInvariant();
		return ReferenceEquals(this, other) || InternalEquals(other);
	}
	public override bool Equals(object? obj) => obj is WeakObjectPtrBase other && Equals(other);
	
	public override int32 GetHashCode()
	{
		MasterAlcCache.GuardInvariant();
		return InternalGetHashCode();
	}
	
	public static bool operator ==(WeakObjectPtrBase? left, WeakObjectPtrBase? right) => Equals(left, right);
	public static bool operator !=(WeakObjectPtrBase? left, WeakObjectPtrBase? right) => !Equals(left, right);
	
	protected WeakObjectPtrBase(){}
	protected WeakObjectPtrBase(IntPtr unmanaged) : base(unmanaged){}

	protected override unsafe bool InternalIsValid(bool evenIfGarbage)
		=> WeakObjectPtr_Interop.IsValid(ConjugateHandle.FromConjugate(this), Convert.ToByte(evenIfGarbage)) > 0;

	protected override unsafe bool InternalIsNull()
		=> WeakObjectPtr_Interop.IsNull(ConjugateHandle.FromConjugate(this)) > 0;

	private unsafe bool InternalEquals(WeakObjectPtrBase? other)
		=> other is not null && WeakObjectPtr_Interop.Identical(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(other)) > 0;
	
	private unsafe int32 InternalGetHashCode()
		=> WeakObjectPtr_Interop.Hash(ConjugateHandle.FromConjugate(this));
	
}


