// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine.Core;

[ConjugateRegistryId(42)]
public abstract class UnrealMulticastInlineDelegateBase : UnrealDynamicDelegateBase
{
	
	public void Add(UnrealObject obj, string name) => this.ZCall("ex://MulticastInlineDelegate.AddUFunction", obj, new UnrealName(name));
	public void Remove(UnrealObject obj, string name) => this.ZCall("ex://MulticastInlineDelegate.RemoveUFunction", obj, new UnrealName(name));
	public void RemoveAll(UnrealObject obj) => this.ZCall("ex://MulticastInlineDelegate.RemoveAll", obj);
	public void Clear() => this.ZCall("ex://MulticastInlineDelegate.Clear");
	public DynamicZCallResult Broadcast(params object?[] parameters) => this.ZCall("ex://MulticastInlineDelegate.Broadcast", parameters);
	
	public bool IsBound => this.ZCall("ex://MulticastInlineDelegate.IsBound", false)[-1].Bool;
	
	protected UnrealMulticastInlineDelegateBase(Type delegateType)
	{
		_delegateType = delegateType;
		
		Unmanaged = MasterAlcCache.Instance.BuildConjugate(this, UnrealDelegateBase.GetUnrealDelegateSignature(_delegateType).Unmanaged);
	}

	protected UnrealMulticastInlineDelegateBase(Type delegateType, IntPtr unmanaged) : base(unmanaged)
	{
		_delegateType = delegateType;
	}
	
	protected UnrealObject Add(Delegate @delegate)
	{
		check(@delegate.GetType() == _delegateType);

		GCHandle handle = GCHandle.Alloc(@delegate);
		return this.ZCall("ex://MulticastInlineDelegate.AddManaged", handle, null)[-1].ReadConjugate<UnrealObject>()!;
	}
	
	private readonly Type _delegateType;
	
}


