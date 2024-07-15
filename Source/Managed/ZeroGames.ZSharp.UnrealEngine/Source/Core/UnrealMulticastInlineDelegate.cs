// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine.Core;

[ConjugateRegistryId(42)]
public abstract class UnrealMulticastInlineDelegate : PlainExportedObjectBase
{
	
	public void Add(UnrealObject obj, string name) => this.ZCall("ex://MulticastInlineDelegate.AddUFunction", obj, new UnrealName(name));
	public void Remove(UnrealObject obj, string name) => this.ZCall("ex://MulticastInlineDelegate.RemoveUFunction", obj, new UnrealName(name));
	public void RemoveAll(UnrealObject obj) => this.ZCall("ex://MulticastInlineDelegate.RemoveAll", obj);
	public void Clear() => this.ZCall("ex://MulticastInlineDelegate.Clear");
	public DynamicZCallResult Broadcast(params object?[] parameters) => this.ZCall("ex://MulticastInlineDelegate.Broadcast", parameters);
	
	public bool IsBound => this.ZCall("ex://MulticastInlineDelegate.IsBound", false)[-1].Bool;
	
	protected UnrealMulticastInlineDelegate(Type delegateType)
	{
		_delegateType = delegateType;
		ValidateDelegateType();
		
		Unmanaged = GetOwningAlc().BuildConjugate(this, UnrealDelegate.GetUnrealDelegateSignature(_delegateType).Unmanaged);
	}

	protected UnrealMulticastInlineDelegate(Type delegateType, IntPtr unmanaged) : base(unmanaged)
	{
		_delegateType = delegateType;
		ValidateDelegateType();
	}
	
	protected UnrealObject? Add(Delegate @delegate)
	{
		if (@delegate.GetType() != _delegateType)
		{
			throw new InvalidOperationException();
		}

		GCHandle handle = GCHandle.Alloc(@delegate);
		return this.ZCall("ex://MulticastInlineDelegate.AddManaged", handle, null)[-1].ReadConjugate<UnrealObject>();
	}
	
	private void ValidateDelegateType()
	{
		if (!UnrealDelegate.IsUnrealDelegateType(_delegateType))
		{
			throw new NotSupportedException();
		}
	}
	
	private readonly Type _delegateType;
	
}

public class UnrealMulticastInlineDelegate<T> : UnrealMulticastInlineDelegate, IConjugate<UnrealMulticastInlineDelegate<T>> where T : Delegate
{
	
	public static UnrealMulticastInlineDelegate<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);

	public UnrealMulticastInlineDelegate() : base(typeof(T)){}
	public UnrealMulticastInlineDelegate(IntPtr unmanaged) : base(typeof(T), unmanaged){}
	
	public UnrealObject? Add(T @delegate) => base.Add(@delegate);
	
}


