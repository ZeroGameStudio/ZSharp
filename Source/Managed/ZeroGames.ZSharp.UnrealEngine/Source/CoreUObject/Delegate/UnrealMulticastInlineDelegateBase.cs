// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

[ConjugateRegistryId(42)]
public abstract class UnrealMulticastInlineDelegateBase : UnrealExportedObjectBase
{
	
	public void Add(UnrealObject obj, string name) => this.ZCall(MasterAlcCache.Instance, "ex://MulticastInlineDelegate.AddUFunction", obj, new UnrealName(name));
	public void Remove(UnrealObject obj, string name) => this.ZCall(MasterAlcCache.Instance, "ex://MulticastInlineDelegate.RemoveUFunction", obj, new UnrealName(name));
	public void RemoveAll(UnrealObject obj) => this.ZCall(MasterAlcCache.Instance, "ex://MulticastInlineDelegate.RemoveAll", obj);
	public void Clear() => this.ZCall(MasterAlcCache.Instance, "ex://MulticastInlineDelegate.Clear");
	public DynamicZCallResult Broadcast(params ReadOnlySpan<object?> parameters) => this.ZCall(MasterAlcCache.Instance, "ex://MulticastInlineDelegate.Broadcast", parameters);
	
	public bool IsBound => this.ZCall(MasterAlcCache.Instance, "ex://MulticastInlineDelegate.IsBound", false)[-1].Bool;
	
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
		return this.ZCall(MasterAlcCache.Instance, "ex://MulticastInlineDelegate.AddManaged", handle, null)[-1].ReadConjugateChecked<UnrealObject>();
	}
	
	private readonly Type _delegateType;
	
}


