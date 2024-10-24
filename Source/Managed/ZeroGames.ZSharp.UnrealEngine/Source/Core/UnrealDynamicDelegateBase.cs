﻿// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Core;

public abstract class UnrealDynamicDelegateBase : PlainExportedObjectBase, IUnrealFieldPath
{
	
	public UnrealDynamicDelegateBase(){}
	protected UnrealDynamicDelegateBase(IntPtr unmanaged) : base(unmanaged){}
	
	public abstract string UnrealFieldPath { get; }
	
}


