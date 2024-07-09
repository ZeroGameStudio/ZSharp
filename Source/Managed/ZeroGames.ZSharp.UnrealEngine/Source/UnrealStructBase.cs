// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine;

[ConjugateRegistryId(2)]
public abstract class UnrealStructBase : UnrealExportedObjectBase, IStaticStruct
{
	
    public UnrealStructBase(){}
    protected UnrealStructBase(IntPtr unmanaged) : base(unmanaged){}

    public static UnrealScriptStruct SStaticStruct => throw new NotSupportedException();
    
}


