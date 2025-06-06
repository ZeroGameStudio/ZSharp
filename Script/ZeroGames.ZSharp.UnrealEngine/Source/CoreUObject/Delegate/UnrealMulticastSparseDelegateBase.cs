﻿// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

// IMPORTANT: Type name and namespace is used by magic, DO NOT change!
[ConjugateRegistryId(43)]
public abstract class UnrealMulticastSparseDelegateBase : UnrealConjugateBase, IUnrealFieldPath
{
	
	public void Add(UObject? obj, string? name)
	{
		if (obj is null || string.IsNullOrWhiteSpace(name))
		{
			return;
		}
		
		MasterAlcCache.GuardInvariant();
		InternalAdd(obj, name);
	}

	public void Remove(UObject? obj, string? name)
	{
		if (obj is null || string.IsNullOrWhiteSpace(name))
		{
			return;
		}
		
		MasterAlcCache.GuardInvariant();
		InternalRemove(obj, name);
	}

	public void RemoveAll(UObject? obj)
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

	public bool IsBoundToObject(UObject? obj)
	{
		if (obj is null)
		{
			return false;
		}
		
		MasterAlcCache.GuardInvariant();
		return InternalIsBoundToObject(obj);
	}

	public bool Contains(UObject? obj, string? name)
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
	
	public abstract string UnrealFieldPath { get; }
	
	protected UnrealMulticastSparseDelegateBase(){}
	protected UnrealMulticastSparseDelegateBase(IntPtr unmanaged) : base(unmanaged){}
		
	protected unsafe EZCallErrorCode Broadcast(ZCallBuffer* buffer) => UnrealMulticastSparseDelegate_Interop.Broadcast(buffer);
	protected unsafe UObject Add(Delegate @delegate) => UnrealMulticastSparseDelegate_Interop.AddStatelessManagedDelegate(ConjugateHandle.FromConjugate(this), GCHandle.Alloc(@delegate)).GetTargetChecked<UObject>();
	protected unsafe UObject Add(Delegate @delegate, object? state) => UnrealMulticastSparseDelegate_Interop.AddStatefulManagedDelegate(ConjugateHandle.FromConjugate(this), GCHandle.Alloc(@delegate), GCHandle.Alloc(state)).GetTargetChecked<UObject>();

	private unsafe void InternalAdd(UObject obj, string name)
	{
		fixed (char* nameBuffer = name)
		{
			UnrealMulticastSparseDelegate_Interop.AddUnrealFunction(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(obj), nameBuffer);
		}
	}

	private unsafe void InternalRemove(UObject obj, string name)
	{
		fixed (char* nameBuffer = name)
		{
			UnrealMulticastSparseDelegate_Interop.Remove(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(obj), nameBuffer);
		}
	}

	private unsafe void InternalRemoveAll(UObject obj) => UnrealMulticastSparseDelegate_Interop.RemoveAll(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(obj));
	private unsafe void InternalClear() => UnrealMulticastSparseDelegate_Interop.Clear(ConjugateHandle.FromConjugate(this));
	private unsafe bool InternalIsBoundToObject(UObject obj) => UnrealMulticastSparseDelegate_Interop.IsBoundToObject(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(obj)) > 0;
	private unsafe bool InternalContains(UObject obj, string name)
	{
		fixed (char* nameBuffer = name)
		{
			return UnrealMulticastSparseDelegate_Interop.Contains(ConjugateHandle.FromConjugate(this), ConjugateHandle.FromConjugate(obj), nameBuffer) > 0;
		}
	}

	private unsafe bool InternalIsBound => UnrealMulticastSparseDelegate_Interop.IsBound(ConjugateHandle.FromConjugate(this)) > 0;
	
}


