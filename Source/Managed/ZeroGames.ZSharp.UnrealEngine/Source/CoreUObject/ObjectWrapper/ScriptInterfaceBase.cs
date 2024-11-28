// Copyright Zero Games. All Rights Reserved.

using System.Numerics;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

public abstract class ScriptInterfaceBase : PlainExportedObjectBase
	, IEquatable<ScriptInterfaceBase>
	, IEqualityOperators<ScriptInterfaceBase?, ScriptInterfaceBase?, bool>
{
	
	public bool Equals(ScriptInterfaceBase? other) => ReferenceEquals(this, other) || InternalEquals(other);
	public override bool Equals(object? obj) => obj is ScriptInterfaceBase other && Equals(other);
	public override int32 GetHashCode() => InternalGetHashCode();
	
	public static bool operator ==(ScriptInterfaceBase? left, ScriptInterfaceBase? right) => Equals(left, right);
	public static bool operator !=(ScriptInterfaceBase? left, ScriptInterfaceBase? right) => !Equals(left, right);
	
	protected ScriptInterfaceBase(){}
	protected ScriptInterfaceBase(IntPtr unmanaged) : base(unmanaged){}

	private unsafe bool InternalEquals(ScriptInterfaceBase? other)
	{
		Thrower.ThrowIfNotInGameThread();
		return other is not null && ScriptInterface_Interop.Identical(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(other)) > 0;
	}
	
	private unsafe int32 InternalGetHashCode()
	{
		Thrower.ThrowIfNotInGameThread();
		return ScriptInterface_Interop.Hash(ConjugateHandle.FromConjugate(this));
	}
	
}