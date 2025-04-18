﻿// Copyright Zero Games. All Rights Reserved.

using System.Numerics;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public abstract class SoftClassPtrBase : UnrealObjectWrapperBase
	, IEquatable<SoftClassPtrBase>
	, IEqualityOperators<SoftClassPtrBase?, SoftClassPtrBase?, bool>
{
	
	public bool Equals(SoftClassPtrBase? other)
	{
		MasterAlcCache.GuardInvariant();
		return ReferenceEquals(this, other) || InternalEquals(other);
	}
	public override bool Equals(object? obj) => obj is SoftClassPtrBase other && Equals(other);
	
	public override int32 GetHashCode()
	{
		MasterAlcCache.GuardInvariant();
		return InternalGetHashCode();
	}
	
	public static bool operator ==(SoftClassPtrBase? left, SoftClassPtrBase? right) => Equals(left, right);
	public static bool operator !=(SoftClassPtrBase? left, SoftClassPtrBase? right) => !Equals(left, right);

	protected SoftClassPtrBase(){}
	protected SoftClassPtrBase(IntPtr unmanaged) : base(unmanaged){}
		
	protected override unsafe bool InternalIsValid(bool evenIfGarbage)
		=> SoftClassPtr_Interop.IsValid(ConjugateHandle.FromConjugate(this), Convert.ToByte(evenIfGarbage)) > 0;

	protected override unsafe bool InternalIsNull()
		=> SoftClassPtr_Interop.IsNull(ConjugateHandle.FromConjugate(this)) > 0;

	private unsafe bool InternalEquals(SoftClassPtrBase? other)
		=> other is not null && SoftClassPtr_Interop.Identical(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(other)) > 0;
	
	private unsafe int32 InternalGetHashCode()
		=> SoftClassPtr_Interop.Hash(ConjugateHandle.FromConjugate(this));
	
}