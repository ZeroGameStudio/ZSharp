﻿// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.InteropServices;
using ZeroGames.ZSharp.Core.UnrealEngine.Specifier;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

[ConjugateRegistryId(41)]
public abstract class UnrealDelegateBase : UnrealDynamicDelegateBase
{

	public static UnrealFunction GetUnrealDelegateSignature(Type t)
	{
		verify(t.GetCustomAttribute<UnrealFieldPathAttribute>() is var attr && attr is not null);
		return LowLevelFindObject<UnrealFunction>(attr.Path)!;
	}

	public void Bind(UnrealObject obj, string name) => this.ZCall(MasterAlcCache.Instance, "ex://Delegate.BindUFunction", obj, new UnrealName(name));
	public void Unbind() => this.ZCall(MasterAlcCache.Instance, "ex://Delegate.Unbind");
	public DynamicZCallResult Execute(params ReadOnlySpan<object?> parameters) => this.ZCall(MasterAlcCache.Instance, "ex://Delegate.Execute", parameters);
	public bool IsBoundToObject(UnrealObject obj) => this.ZCall(MasterAlcCache.Instance, "ex://Delegate.IsBoundToObject", obj, false)[-1].Bool;
	
	public UnrealObject? Object => this.ZCall(MasterAlcCache.Instance, "ex://Delegate.GetObject", [ null ])[-1].ReadConjugate<UnrealObject>();
	public string FunctionName => this.ZCall(MasterAlcCache.Instance, "ex://Delegate.GetFunctionName", [null])[-1].ReadConjugateChecked<UnrealName>().ToString();
	public bool IsBound => this.ZCall(MasterAlcCache.Instance, "ex://Delegate.IsBound", false)[-1].Bool;
	
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
		return this.ZCall(MasterAlcCache.Instance, "ex://Delegate.BindManaged", handle, null)[-1].ReadConjugateChecked<UnrealObject>();
	}
	
	private readonly Type _delegateType;

}

