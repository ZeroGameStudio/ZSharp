// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

[ConjugateRegistryId(43)]
public abstract class UnrealMulticastSparseDelegateBase : UnrealDynamicDelegateBase
{
	
	public void Add(UnrealObject obj, string name) => this.ZCall(MasterAlcCache.Instance, "ex://MulticastSparseDelegate.AddUFunction", obj, new UnrealName(name));
	public void Remove(UnrealObject obj, string name) => this.ZCall(MasterAlcCache.Instance, "ex://MulticastSparseDelegate.RemoveUFunction", obj, new UnrealName(name));
	public void RemoveAll(UnrealObject obj) => this.ZCall(MasterAlcCache.Instance, "ex://MulticastSparseDelegate.RemoveAll", obj);
	public void Clear() => this.ZCall(MasterAlcCache.Instance, "ex://MulticastSparseDelegate.Clear");
	public DynamicZCallResult Broadcast(params ReadOnlySpan<object?> parameters) => this.ZCall(MasterAlcCache.Instance, "ex://MulticastSparseDelegate.Broadcast", parameters);
	
	public bool IsBound => this.ZCall(MasterAlcCache.Instance, "ex://MulticastSparseDelegate.IsBound", false)[-1].Bool;
	
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
		return this.ZCall(MasterAlcCache.Instance, "ex://MulticastSparseDelegate.AddManaged", handle, null)[-1].ReadConjugateChecked<UnrealObject>();
	}
	
	private readonly Type _delegateType;
	
}


