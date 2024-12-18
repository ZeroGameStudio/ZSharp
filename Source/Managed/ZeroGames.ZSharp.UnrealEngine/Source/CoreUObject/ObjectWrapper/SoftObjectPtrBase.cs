// Copyright Zero Games. All Rights Reserved.

using System.Numerics;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public abstract class SoftObjectPtrBase : UnrealConjugateBase
	, IEquatable<SoftObjectPtrBase>
	, IEqualityOperators<SoftObjectPtrBase?, SoftObjectPtrBase?, bool>
	, IUnrealObjectPath
{
	
	public bool Equals(SoftObjectPtrBase? other) => ReferenceEquals(this, other) || InternalEquals(other);
	public override bool Equals(object? obj) => obj is SoftObjectPtrBase other && Equals(other);
	public override int32 GetHashCode() => InternalGetHashCode();
	
	public static bool operator ==(SoftObjectPtrBase? left, SoftObjectPtrBase? right) => Equals(left, right);
	public static bool operator !=(SoftObjectPtrBase? left, SoftObjectPtrBase? right) => !Equals(left, right);
	
	public string Path => throw new NotImplementedException();
	
	protected SoftObjectPtrBase(){}
	protected SoftObjectPtrBase(IntPtr unmanaged) : base(unmanaged){}

	private unsafe bool InternalEquals(SoftObjectPtrBase? other)
	{
		Thrower.ThrowIfNotInGameThread();
		MasterAlcCache.Instance.GuardUnloaded();
		return other is not null && SoftObjectPtr_Interop.Identical(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(other)) > 0;
	}
	
	private unsafe int32 InternalGetHashCode()
	{
		Thrower.ThrowIfNotInGameThread();
		MasterAlcCache.Instance.GuardUnloaded();
		return SoftObjectPtr_Interop.Hash(ConjugateHandle.FromConjugate(this));
	}
	
}