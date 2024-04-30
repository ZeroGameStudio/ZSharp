// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.InteropServices;
using System.Runtime.Loader;
using ZeroGames.ZSharp.Core;
using ZeroGames.ZSharp.UnrealEngine.Core;

namespace ZeroGames.ZSharp.UnrealEngine.Export;

/**
 * Unmanaged Lifecycle:
 *     AllocUnmanagedResource (done by unmanaged code)
 *     -> ...
 *     -> ZCall
 *     -> AllocManagedObject & BuildConjugate (request by unmanaged code)
 *     -> ...
 *     -> ReleaseUnmanagedResource (done by unmanaged code somewhen)
 *     -> MarkAsDead (request by unmanaged code from ReleaseConjugate)
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
 *     -> MarkAsDead (request directly by finalizer)
 *     -> ...
 *     -> ReleaseManagedObject (done by GC)
 */
public abstract class ExportedObjectBase : IConjugate
{
    
    public static MasterAssemblyLoadContext GetOwningALC()
    {
        Assembly asm = typeof(ExportedObjectBase).Assembly;
        AssemblyLoadContext? alc = AssemblyLoadContext.GetLoadContext(asm);
        if (alc is null)
        {
            throw new Exception("Owning ALC not found.");
        }

        if (alc is MasterAssemblyLoadContext masterAlc)
        {
            if (alc != MasterAssemblyLoadContext.Get())
            {
                throw new Exception("Owning ALC is MasterAssemblyLoadContext but not the live one.");
            }
            
            return masterAlc;
        }

        throw new Exception($"Owning ALC is not MasterAssemblyLoadContext but {alc.GetType().Name}");
    }

    public void ReleaseConjugate()
    {
        if (_bManaged)
        {
            Logger.Error("Dispose managed exported object from unmanaged code.");
            return;
        }
        
        if (!this.IsValid())
        {
            Logger.Error("Dispose exported object twice.");
            return;
        }
        
        MarkAsDead();
        
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
        MarkAsDead();
    }

    private void MarkAsDead() => GCHandle.Free();

    private readonly bool _bManaged;

}


