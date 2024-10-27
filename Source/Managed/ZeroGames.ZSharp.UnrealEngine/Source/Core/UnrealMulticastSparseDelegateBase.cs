// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine.Core;

[ConjugateRegistryId(43)]
public abstract class UnrealMulticastSparseDelegateBase : UnrealDynamicDelegateBase
{
	
	public void Add(UnrealObject obj, string name) => this.ZCall("ex://MulticastSparseDelegate.AddUFunction", obj, new UnrealName(name));
	public void Remove(UnrealObject obj, string name) => this.ZCall("ex://MulticastSparseDelegate.RemoveUFunction", obj, new UnrealName(name));
	public void RemoveAll(UnrealObject obj) => this.ZCall("ex://MulticastSparseDelegate.RemoveAll", obj);
	public void Clear() => this.ZCall("ex://MulticastSparseDelegate.Clear");
	public DynamicZCallResult Broadcast(params object?[] parameters) => this.ZCall("ex://MulticastSparseDelegate.Broadcast", parameters);
	
	public bool IsBound => this.ZCall("ex://MulticastSparseDelegate.IsBound", false)[-1].Bool;
	
	protected UnrealMulticastSparseDelegateBase(Type delegateType)
	{
		_delegateType = delegateType;
		
		Unmanaged = MasterAlcCache.Instance.BuildConjugate(this, UnrealDelegateBase.GetUnrealDelegateSignature(_delegateType).Unmanaged);
	}

	protected UnrealMulticastSparseDelegateBase(Type delegateType, IntPtr unmanaged) : base(unmanaged)
	{
		_delegateType = delegateType;
	}
	
	protected UnrealObject Add(Delegate @delegate)
	{
		check(@delegate.GetType() == _delegateType);

		GCHandle handle = GCHandle.Alloc(@delegate);
		return this.ZCall("ex://MulticastSparseDelegate.AddManaged", handle, null)[-1].ReadConjugate<UnrealObject>()!;
	}
	
	private readonly Type _delegateType;
	
}


