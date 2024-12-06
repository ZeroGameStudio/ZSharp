// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

[ConjugateRegistryId(42)]
public abstract class UnrealMulticastInlineDelegateBase : UnrealExportedObjectBase
{

	public void Add(UnrealObject? obj, string? name)
	{
		if (obj is null || string.IsNullOrWhiteSpace(name))
		{
			return;
		}
		
		MasterAlcCache.GuardInvariant();
		InternalAdd(obj, name);
	}

	public void Remove(UnrealObject? obj, string? name)
	{
		if (obj is null || string.IsNullOrWhiteSpace(name))
		{
			return;
		}
		
		MasterAlcCache.GuardInvariant();
		InternalRemove(obj, name);
	}

	public void RemoveAll(UnrealObject? obj)
	{
		if (obj is null)
		{
			return;
		}
		
		MasterAlcCache.GuardInvariant();
		InternalRemoveAll(obj);
	}

	public void Clear()
	{
		MasterAlcCache.GuardInvariant();
		InternalClear();
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

	public bool Contains(UnrealObject? obj, string? name)
	{
		if (obj is null || string.IsNullOrWhiteSpace(name))
		{
			return false;
		}
		
		MasterAlcCache.GuardInvariant();
		return InternalContains(obj, name);
	}

	public bool IsBound
	{
		get
		{
			MasterAlcCache.GuardInvariant();
			return InternalIsBound;
		}
	}
	
	protected UnrealMulticastInlineDelegateBase(){}
	protected UnrealMulticastInlineDelegateBase(IntPtr unmanaged) : base(unmanaged){}
	
	private unsafe void InternalAdd(UnrealObject obj, string name)
	{
		fixed (char* nameBuffer = name)
		{
			UnrealMulticastInlineDelegate_Interop.AddUnrealFunction(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(obj), nameBuffer);
		}
	}

	private unsafe void InternalRemove(UnrealObject obj, string name)
	{
		fixed (char* nameBuffer = name)
		{
			UnrealMulticastInlineDelegate_Interop.Remove(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(obj), nameBuffer);
		}
	}

	private unsafe void InternalRemoveAll(UnrealObject obj) => UnrealMulticastInlineDelegate_Interop.RemoveAll(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(obj));
	private unsafe void InternalClear() => UnrealMulticastInlineDelegate_Interop.Clear(ConjugateHandle.FromConjugate(this));
	private unsafe bool InternalIsBoundToObject(UnrealObject obj) => UnrealMulticastInlineDelegate_Interop.IsBoundToObject(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(obj)) > 0;
	private unsafe bool InternalContains(UnrealObject obj, string name)
	{
		fixed (char* nameBuffer = name)
		{
			return UnrealMulticastInlineDelegate_Interop.Contains(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(obj), nameBuffer) > 0;
		}
	}

	private unsafe bool InternalIsBound => UnrealMulticastInlineDelegate_Interop.IsBound(ConjugateHandle.FromConjugate(this)) > 0;
	
}


