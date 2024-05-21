// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine;

[ConjugateRegistryId(2)]
public abstract class UnrealStructBase : ExportedObjectBase
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

}


