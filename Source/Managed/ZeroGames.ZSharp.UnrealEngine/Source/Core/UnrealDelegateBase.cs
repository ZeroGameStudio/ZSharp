// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.InteropServices;
using ZeroGames.ZSharp.Core.UnrealEngine.Specifier;

namespace ZeroGames.ZSharp.UnrealEngine.Core;

[ConjugateRegistryId(41)]
public abstract class UnrealDelegateBase : UnrealDynamicDelegateBase
{

	public static UnrealFunction GetUnrealDelegateSignature(Type t)
	{
		verify(t.GetCustomAttribute<UnrealFieldPathAttribute>() is var attr && attr is not null);
		return LowLevelFindObject<UnrealFunction>(attr.Path)!;
	}

	public void Bind(UnrealObject obj, string name) => this.ZCall("ex://Delegate.BindUFunction", obj, new UnrealName(name));
	public void Unbind() => this.ZCall("ex://Delegate.Unbind");
	public DynamicZCallResult Execute(params object?[] parameters) => this.ZCall("ex://Delegate.Execute", parameters);
	public bool IsBoundToObject(UnrealObject obj) => this.ZCall("ex://Delegate.IsBoundToObject", obj, false)[-1].Bool;
	
	public UnrealObject? Object => this.ZCall("ex://Delegate.GetObject", [ null ])[-1].ReadConjugate<UnrealObject>();
	public string FunctionName => this.ZCall("ex://Delegate.GetFunctionName", [null])[-1].ReadConjugate<UnrealName>()!.ToString();
	public bool IsBound => this.ZCall("ex://Delegate.IsBound", false)[-1].Bool;
	
	protected UnrealDelegateBase(Type delegateType)
	{
		_delegateType = delegateType;
		
		Unmanaged = MasterAlcCache.Instance.BuildConjugate(this, GetUnrealDelegateSignature(_delegateType).Unmanaged);
	}

	protected UnrealDelegateBase(Type delegateType, IntPtr unmanaged) : base(unmanaged)
	{
		_delegateType = delegateType;
	}

	protected UnrealObject Bind(Delegate @delegate)
	{
		check(@delegate.GetType() == _delegateType);

		GCHandle handle = GCHandle.Alloc(@delegate);
		return this.ZCall("ex://Delegate.BindManaged", handle, null)[-1].ReadConjugate<UnrealObject>()!;
	}
	
	private readonly Type _delegateType;

}


