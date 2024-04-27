// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;
using ZeroGames.ZSharp.Core;

namespace ZeroGames.ZSharp.UnrealEngine.Export;

/**
 * Unmanaged Lifecycle:
 *     AllocUnmanagedResource (done by unmanaged code)
 *     -> ...
 *     -> ZCall
 *     -> AllocManagedObject & BuildConjugate (request by unmanaged code)
 *     -> ...
 *     -> ReleaseUnmanagedResource (done by unmanaged code somewhen)
 *     -> ReleaseConjugate
 *     -> SuppressFinalize (done by ReleaseConjugate)
 *     -> ...
 *     -> ReleaseManagedObject (done by GC)
 *
 * Managed Lifecycle:
 *     AllocManagedObject (request by managed user code)
 *     -> AllocUnmanagedResource & BuildConjugate (request by managed code)
 *     -> ...
 *     -> Finalize
 *     -> ReleaseUnmanagedResource
 *     -> ReleaseConjugate
 *     -> ...
 *     -> ReleaseManagedObject (done by GC)
 */
public abstract class ExportedObjectBase : IConjugate
{

    public void ReleaseConjugate()
    {
        if (!this.IsValid())
        {
            Logger.Error("Dispose exported object twice.");
            return;
        }
        
        GCHandle.Free();
        
        GC.SuppressFinalize(this);
    }

    public GCHandle GCHandle { get; }
    public IntPtr Unmanaged { get; protected init; }

    protected abstract void ReleaseUnmanagedResource();

    private protected ExportedObjectBase()
    {
        GCHandle = GCHandle.Alloc(this, GCHandleType.Weak);
        _bManaged = true;
    }

    private protected ExportedObjectBase(IntPtr unmanaged)
    {
        GCHandle = GCHandle.Alloc(this);
        Unmanaged = unmanaged;
        _bManaged = false;
    }

    ~ExportedObjectBase()
    {
        if (!_bManaged)
        {
            Logger.Error("Finalize unmanaged export object.");
            return;
        }
        
        ReleaseUnmanagedResource();
    }

    private readonly bool _bManaged;

}


