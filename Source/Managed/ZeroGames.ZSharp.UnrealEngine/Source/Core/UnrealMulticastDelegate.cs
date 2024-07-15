// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine.Core;

[ConjugateRegistryId(42)]
public abstract class UnrealMulticastDelegate : PlainExportedObjectBase
{
	
	public void Add(UnrealObject obj, string name) => this.ZCall("ex://MulticastDelegate.AddUFunction", obj, new UnrealName(name));
	public void Remove(UnrealObject obj, string name) => this.ZCall("ex://MulticastDelegate.RemoveUFunction", obj, new UnrealName(name));
	public void RemoveAll(UnrealObject obj) => this.ZCall("ex://MulticastDelegate.RemoveAll", obj);
	public void Clear() => this.ZCall("ex://MulticastDelegate.RemoveAll");
	public DynamicZCallResult Broadcast(params object?[] parameters) => this.ZCall("ex://MulticastDelegate.Broadcast", parameters);
	
	public bool IsBound => this.ZCall("ex://MulticastDelegate.IsBound", false)[-1].Bool;
	
	protected UnrealMulticastDelegate(Type delegateType)
	{
		_delegateType = delegateType;
		ValidateDelegateType();
		
		Unmanaged = GetOwningAlc().BuildConjugate(this, UnrealDelegate.GetUnrealDelegateSignature(_delegateType).Unmanaged);
	}

	protected UnrealMulticastDelegate(Type delegateType, IntPtr unmanaged) : base(unmanaged)
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
		return this.ZCall("ex://MulticastDelegate.AddManaged", handle, null)[-1].ReadConjugate<UnrealObject>();
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

public class UnrealMulticastDelegate<T> : UnrealMulticastDelegate, IConjugate<UnrealMulticastDelegate<T>> where T : Delegate
{
	
	public static UnrealMulticastDelegate<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);

	public UnrealMulticastDelegate() : base(typeof(T)){}
	public UnrealMulticastDelegate(IntPtr unmanaged) : base(typeof(T), unmanaged){}
	
	public UnrealObject? Add(T @delegate) => base.Add(@delegate);
	
}


