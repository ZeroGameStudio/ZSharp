// Copyright Zero Games. All Rights Reserved.

using System.Numerics;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public abstract class ScriptInterfaceBase : UnrealConjugateBase
	, IEquatable<ScriptInterfaceBase>
	, IEqualityOperators<ScriptInterfaceBase?, ScriptInterfaceBase?, bool>
{

	public bool Equals(ScriptInterfaceBase? other)
	{
		MasterAlcCache.GuardInvariant();
		return ReferenceEquals(this, other) || InternalEquals(other);
	}
	public override bool Equals(object? obj) => obj is ScriptInterfaceBase other && Equals(other);

	public override int32 GetHashCode()
	{
		MasterAlcCache.GuardInvariant();
		return InternalGetHashCode();
	}
	
	public static bool operator ==(ScriptInterfaceBase? left, ScriptInterfaceBase? right) => Equals(left, right);
	public static bool operator !=(ScriptInterfaceBase? left, ScriptInterfaceBase? right) => !Equals(left, right);
	
	protected ScriptInterfaceBase(){}
	protected ScriptInterfaceBase(IntPtr unmanaged) : base(unmanaged){}

	private unsafe bool InternalEquals(ScriptInterfaceBase? other)
		=> other is not null && ScriptInterface_Interop.Identical(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(other)) > 0;
	
	private unsafe int32 InternalGetHashCode()
		=> ScriptInterface_Interop.Hash(ConjugateHandle.FromConjugate(this));
	
}