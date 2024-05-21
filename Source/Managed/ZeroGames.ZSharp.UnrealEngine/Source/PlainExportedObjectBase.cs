// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine;

public abstract class PlainExportedObjectBase : ExportedObjectBase
{

    public PlainExportedObjectBase()
    {
        Unmanaged = GetOwningAlc().BuildConjugate(this);
        if (Unmanaged == IntPtr.Zero)
        {
            throw new InvalidOperationException();
        }
    }
    
    protected PlainExportedObjectBase(IntPtr unmanaged) : base(unmanaged){}

}


