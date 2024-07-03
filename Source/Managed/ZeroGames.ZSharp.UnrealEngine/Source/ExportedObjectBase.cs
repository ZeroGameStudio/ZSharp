// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Runtime.Loader;

namespace ZeroGames.ZSharp.UnrealEngine;

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
    
    public static IMasterAssemblyLoadContext GetOwningAlc()
    {
        Assembly asm = typeof(ExportedObjectBase).Assembly;
        AssemblyLoadContext? alc = AssemblyLoadContext.GetLoadContext(asm);
        if (alc is null)
        {
            throw new Exception("Owning ALC not found.");
        }

        if (alc is IMasterAssemblyLoadContext masterAlc)
        {
            if (alc != IMasterAssemblyLoadContext.Get())
            {
                throw new Exception("Owning ALC is MasterAssemblyLoadContext but not the live one.");
            }
            
            return masterAlc;
        }

        throw new Exception($"Owning ALC is not MasterAssemblyLoadContext but {alc.GetType().Name}");
    }

    public void Dispose() => InternalDispose(false);

    public void Release()
    {
        if (_managed)
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
    public IntPtr Unmanaged { get; protected set; }

    private protected ExportedObjectBase()
    {
        GCHandle = GCHandle.Alloc(this, GCHandleType.Weak);
        _managed = true;
    }

    private protected ExportedObjectBase(IntPtr unmanaged)
    {
        GCHandle = GCHandle.Alloc(this);
        Unmanaged = unmanaged;
        _managed = false;
    }

    ~ExportedObjectBase() => InternalDispose(true);

    private void InternalDispose(bool fromFinalizer)
    {
        if (!_managed)
        {
            Logger.Error("Finalize unmanaged export object.");
            return;
        }
        
        if (fromFinalizer)
        {
            GetOwningAlc().PushPendingDisposeConjugate(this);
            return;
        }
        
        if (Volatile.Read(ref _disposed))
        {
            return;
        }
        
        Volatile.Write(ref _disposed, true);
        
        GetOwningAlc().ReleaseConjugate(Unmanaged);
        MarkAsDead();

        if (!fromFinalizer)
        {
            GC.SuppressFinalize(this);
        }
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    private void MarkAsDead()
    {
        GCHandle.Free();
        Unmanaged = IConjugate.KDead;
    }

    private readonly bool _managed;
    private bool _disposed;

}


