// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
[ConjugateRegistryId(41)]
public abstract class UnrealDelegateBase : UnrealConjugateBase, IUnrealFieldPath
{

	public void Bind(UnrealObject? obj, string? name)
	{
		if (obj is null || string.IsNullOrWhiteSpace(name))
		{
			return;
		}
		
		MasterAlcCache.GuardInvariant();
		InternalBind(obj, name);
	}

	public void Unbind()
	{
		MasterAlcCache.GuardInvariant();
		InternalUnbind();
	}

	public bool IsBoundToObject(UnrealObject? obj)
	{
		if (obj is null)
		{
			return false;
		}
		
		MasterAlcCache.GuardInvariant();
		return InternalIsBoundToObject(obj);
	}

	public UnrealObject? Object
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalObject;
		}
	}

	public string FunctionName
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalFunctionName;
		}
	}

	public bool IsBound
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalIsBound;
		}
	}
	
	public abstract string UnrealFieldPath { get; }
	
	protected UnrealDelegateBase(){}
	protected UnrealDelegateBase(IntPtr unmanaged) : base(unmanaged){}

	protected unsafe EZCallErrorCode Execute(ZCallBuffer* buffer) => UnrealDelegate_Interop.Execute(buffer);
	protected unsafe UnrealObject Bind(Delegate @delegate) => UnrealDelegate_Interop.BindStatelessManagedDelegate(ConjugateHandle.FromConjugate(this), GCHandle.Alloc(@delegate)).GetTargetChecked<UnrealObject>();
	protected unsafe UnrealObject Bind(Delegate @delegate, object? state) => UnrealDelegate_Interop.BindStatefulManagedDelegate(ConjugateHandle.FromConjugate(this), GCHandle.Alloc(@delegate), GCHandle.Alloc(state)).GetTargetChecked<UnrealObject>();

	private unsafe void InternalBind(UnrealObject obj, string name)
	{
		fixed (char* nameBuffer = name)
		{
			UnrealDelegate_Interop.BindUnrealFunction(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(obj), nameBuffer);
		}
	}

	private unsafe void InternalUnbind() => UnrealDelegate_Interop.Unbind(ConjugateHandle.FromConjugate(this));
	private unsafe bool InternalIsBoundToObject(UnrealObject obj) => UnrealDelegate_Interop.IsBoundToObject(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(obj)) > 0;
	
	private unsafe UnrealObject? InternalObject => UnrealDelegate_Interop.GetObject(ConjugateHandle.FromConjugate(this)).GetTarget<UnrealObject>();

	private unsafe string InternalFunctionName
	{
		get
		{
			using InteropString buffer = new();
			UnrealDelegate_Interop.GetFunctionName(ConjugateHandle.FromConjugate(this), buffer.Address);
			return buffer;
		}
	}
	
	private unsafe bool InternalIsBound => UnrealDelegate_Interop.IsBound(ConjugateHandle.FromConjugate(this)) > 0;

}


