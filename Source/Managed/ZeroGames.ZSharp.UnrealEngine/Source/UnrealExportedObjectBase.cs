// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine;

public abstract class UnrealExportedObjectBase : ExportedObjectBase, IUnrealFieldPath
{
	
	public static string SUnrealFieldPath => throw new NotSupportedException();

	public UnrealExportedObjectBase(){}
	protected UnrealExportedObjectBase(IntPtr unmanaged) : base(unmanaged){}
	
	public abstract string UnrealFieldPath { get; }
	
}


