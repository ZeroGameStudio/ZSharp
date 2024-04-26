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
        if (_bManaged)
        {
            throw new Exception("Try to dispose managed export object from unmanaged code.");
        }
        
        InternalReleaseConjugate();
        
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
            throw new Exception("Finalize unmanaged export object.");
        }
        
        ReleaseUnmanagedResource();
        InternalReleaseConjugate();
    }

    private void InternalReleaseConjugate()
    {
        if (!this.IsValid())
        {
            throw new Exception("Try to dispose managed export object from unmanaged code.");
        }
        
        GCHandle.Free();
    }

    private readonly bool _bManaged;

}


