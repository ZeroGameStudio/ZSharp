// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

[ConjugateRegistryId(26)]
public abstract class ScriptInterfaceBase : UnrealObjectWrapperBase
{
	
	protected ScriptInterfaceBase(Type objectType) : base(objectType, false, true){}
	protected ScriptInterfaceBase(Type objectType, IntPtr unmanaged) : base(objectType, false, true, unmanaged){}

	public UnrealObject? Object
	{
		get => this.ZCall("ex://Interface.Get", [ null ])[-1].ReadConjugate<UnrealObject>();
		set => this.ZCall("ex://Interface.Set", value);
	}
	
}


