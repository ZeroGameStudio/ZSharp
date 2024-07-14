// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.InteropServices;
using ZeroGames.ZSharp.UnrealEngine.ZSharpRuntime;

namespace ZeroGames.ZSharp.UnrealEngine.Core;

[ConjugateRegistryId(41)]
public abstract class UnrealDelegate : PlainExportedObjectBase
{
	
	public static bool IsUnrealDelegateType(Type t) => t.IsSubclassOf(typeof(Delegate)) && t.GetCustomAttribute<UnrealFieldPathAttribute>() is not null;

	public static UnrealFunction GetUnrealDelegateSignature(Type t)
	{
		if (t.IsSubclassOf(typeof(Delegate)) && t.GetCustomAttribute<UnrealFieldPathAttribute>() is {} attr)
		{
			return UnrealObjectGlobals.LowLevelFindObject<UnrealFunction>(attr.Path)!;
		}

		throw new ArgumentException();
	}

	public void Bind(UnrealObject obj, string name) => this.ZCall("ex://Delegate.BindUFunction", obj, new UnrealName(name));
	public void Unbind() => this.ZCall("ex://Delegate.Unbind");
	public bool IsBoundToObject(UnrealObject obj) => this.ZCall("ex://Delegate.IsBoundToObject", obj)[-1].Bool;

	public void Execute(params object?[] parameters)
	{
		string zcallName = ZCallName;
		if (string.IsNullOrWhiteSpace(zcallName))
		{
			return;
		}

		Object!.ZCall(zcallName, parameters);
	}
	
	public UnrealObject? Object => this.ZCall("ex://Delegate.GetObject")[-1].ReadConjugate<UnrealObject>();
	public string ZCallName => this.ZCall("ex://Delegate.GetZCallName")[-1].ReadConjugate<UnrealString>()!.ToString()!;
	public bool IsBound => this.ZCall("ex://Delegate.IsBound")[-1].Bool;
	
	protected UnrealDelegate(Type delegateType)
	{
		_delegateType = delegateType;
		ValidateDelegateType();
		
		Unmanaged = GetOwningAlc().BuildConjugate(this, GetUnrealDelegateSignature(_delegateType).Unmanaged);
	}

	protected UnrealDelegate(Type delegateType, IntPtr unmanaged) : base(unmanaged)
	{
		_delegateType = delegateType;
		ValidateDelegateType();
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
	
	private void ValidateDelegateType()
	{
		if (!IsUnrealDelegateType(_delegateType))
		{
			throw new NotSupportedException();
		}
	}
	
	private readonly Type _delegateType;
	
}

public class UnrealDelegate<T> : UnrealDelegate, IConjugate<UnrealDelegate<T>> where T : Delegate
{
	
	public static UnrealDelegate<T> BuildConjugate(IntPtr unmanaged) => new(unmanaged);

	public UnrealDelegate() : base(typeof(T)){}
	public UnrealDelegate(IntPtr unmanaged) : base(typeof(T), unmanaged){}

	public UnrealObject? Bind(T @delegate) => base.Bind(@delegate);

}


