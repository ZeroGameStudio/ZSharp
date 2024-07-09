// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine;

[ConjugateRegistryId(1)]
public abstract class UnrealObjectBase : UnrealExportedObjectBase, IStaticClass
{

    protected UnrealObjectBase(IntPtr unmanaged) : base(unmanaged){}

    public static UnrealClass SStaticClass => throw new NotSupportedException();
    
}


