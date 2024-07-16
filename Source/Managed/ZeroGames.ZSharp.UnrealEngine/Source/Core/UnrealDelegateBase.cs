// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine.Core;

[ConjugateRegistryId(41)]
public abstract class UnrealDelegateBase : PlainExportedObjectBase
{

	public static UnrealFunction GetUnrealDelegateSignature(Type t)
	{
		if (t.GetCustomAttribute<UnrealFieldPathAttribute>() is {} attr)
		{
			return UnrealObjectGlobals.LowLevelFindObject<UnrealFunction>(attr.Path)!;
		}

		throw new ArgumentException();
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
		
		Unmanaged = GetOwningAlc().BuildConjugate(this, GetUnrealDelegateSignature(_delegateType).Unmanaged);
	}

	protected UnrealDelegateBase(Type delegateType, IntPtr unmanaged) : base(unmanaged)
	{
		_delegateType = delegateType;
	}

	protected UnrealObject? Bind(Delegate @delegate)
	{
		if (@delegate.GetType() != _delegateType)
		{
			throw new InvalidOperationException();
		}

		GCHandle handle = GCHandle.Alloc(@delegate);
		return this.ZCall("ex://Delegate.BindManaged", handle, null)[-1].ReadConjugate<UnrealObject>();
	}
	
	private readonly Type _delegateType;

}


