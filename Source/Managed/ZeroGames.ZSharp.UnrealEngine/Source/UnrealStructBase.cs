// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine;

[ConjugateRegistryId(2)]
public abstract class UnrealStructBase : ExportedObjectBase, IStaticStruct
{

    public unsafe UnrealStructBase()
    {
	    UnrealScriptStruct scriptStruct = (UnrealScriptStruct)GetType().GetProperty(nameof(IStaticStruct.SStaticStruct))?.GetValue(null)!;
	    Unmanaged = GetOwningAlc().BuildConjugate(this, (void*)scriptStruct.Unmanaged);
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


