// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine.Core;

[ConjugateRegistryId(43)]
public abstract class UnrealMulticastSparseDelegate : PlainExportedObjectBase
{
	
	public void Add(UnrealObject obj, string name) => this.ZCall("ex://MulticastSparseDelegate.AddUFunction", obj, new UnrealName(name));
	public void Remove(UnrealObject obj, string name) => this.ZCall("ex://MulticastSparseDelegate.RemoveUFunction", obj, new UnrealName(name));
	public void RemoveAll(UnrealObject obj) => this.ZCall("ex://MulticastSparseDelegate.RemoveAll", obj);
	public void Clear() => this.ZCall("ex://MulticastSparseDelegate.Clear");
	public DynamicZCallResult Broadcast(params object?[] parameters) => this.ZCall("ex://MulticastSparseDelegate.Broadcast", parameters);
	
	public bool IsBound => this.ZCall("ex://MulticastSparseDelegate.IsBound", false)[-1].Bool;
	
	protected UnrealMulticastSparseDelegate(Type delegateType)
	{
		_delegateType = delegateType;
		ValidateDelegateType();
		
		Unmanaged = GetOwningAlc().BuildConjugate(this, UnrealDelegate.GetUnrealDelegateSignature(_delegateType).Unmanaged);
	}

	protected UnrealMulticastSparseDelegate(Type delegateType, IntPtr unmanaged) : base(unmanaged)
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
		return this.ZCall("ex://MulticastSparseDelegate.AddManaged", handle, null)[-1].ReadConjugate<UnrealObject>();
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

public class UnrealMulticastSparseDelegate<T> : UnrealMulticastSparseDelegate, IConjugate<UnrealMulticastSparseDelegate<T>> where T : Delegate
{
	
	public static UnrealMulticastSparseDelegate<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);

	public UnrealMulticastSparseDelegate() : base(typeof(T)){}
	public UnrealMulticastSparseDelegate(IntPtr unmanaged) : base(typeof(T), unmanaged){}
	
	public UnrealObject? Add(T @delegate) => base.Add(@delegate);
	
}


