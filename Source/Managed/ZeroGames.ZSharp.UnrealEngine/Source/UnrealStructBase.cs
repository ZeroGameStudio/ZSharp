// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine;

[ConjugateRegistryId(2)]
public abstract class UnrealStructBase : ExportedObjectBase, IStaticStruct
{

    public UnrealStructBase()
    {
	    Unmanaged = GetOwningAlc().BuildConjugate(this);
	    if (Unmanaged == IntPtr.Zero)
	    {
		    throw new InvalidOperationException();
	    }
    }
    
    protected UnrealStructBase(IntPtr unmanaged) : base(unmanaged){}

    public static string SUnrealFieldPath => throw new NotSupportedException();
    public static UnrealScriptStruct SStaticStruct => throw new NotSupportedException();
    
    public abstract string UnrealFieldPath { get; }
    
}


