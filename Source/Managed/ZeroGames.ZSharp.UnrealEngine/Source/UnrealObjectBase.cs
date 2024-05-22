// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine;

[ConjugateRegistryId(1)]
public abstract class UnrealObjectBase : ExportedObjectBase, IStaticClass
{

    protected UnrealObjectBase(IntPtr unmanaged) : base(unmanaged){}

    public static string SUnrealFieldPath => throw new NotSupportedException();
    public static UnrealClass SStaticClass => throw new NotSupportedException();
    
    public abstract string UnrealFieldPath { get; }
    
}


