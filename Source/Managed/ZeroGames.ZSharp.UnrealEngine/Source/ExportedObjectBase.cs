// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Runtime.Loader;
using System.Threading;

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
        if (_isManaged)
        {
            Logger.Error("Dispose managed exported object from unmanaged code.");
            return;
        }
        
        if (Unmanaged == IConjugate.KDead)
        {
            Logger.Error("Dispose exported object twice.");
            return;
        }
        
        MarkAsDead();
        
        GC.SuppressFinalize(this);
    }
    
    public ExplicitLifecycleExpiredRegistration RegisterOnExpired(Action<IExplicitLifecycle, object?> callback, object? state)
    {
        lock (this)
        {
            if (IsExpired)
            {
                if (UnrealEngineStatics.IsInGameThread)
                {
                    callback(this, state);
                }
                else
                {
                    GetOwningAlc().SynchronizationContext.Send(s => callback(this, s), state);
                }
                
                return default;
            }
            else
            {
                _onExpiredRegistry ??= new();
                ExplicitLifecycleExpiredRegistration reg = new(this, ++_onExpiredRegistrationHandle);
                _onExpiredRegistry[reg] = new(callback, state);

                return reg;
            }
        }
    }

    public void UnregisterOnExpired(ExplicitLifecycleExpiredRegistration registration)
    {
        lock (this)
        {
            if (!IsExpired)
            {
                _onExpiredRegistry?.Remove(registration);
            }
        }
    }

    public GCHandle GCHandle { get; }

    public IntPtr Unmanaged
    {
        get => _unmanaged;
        protected set
        {
            lock (this)
            {
                _unmanaged = value;
            }
        }
    }

    public bool IsExpired
    {
        get
        {
            lock (this)
            {
                return Unmanaged == IConjugate.KDead;
            }
        }
    }

    private protected ExportedObjectBase()
    {
        GCHandle = GCHandle.Alloc(this, GCHandleType.Weak);
        _isManaged = true;
    }

    private protected ExportedObjectBase(IntPtr unmanaged)
    {
        GCHandle = GCHandle.Alloc(this);
        Unmanaged = unmanaged;
        _isManaged = false;
    }

    ~ExportedObjectBase() => InternalDispose(true);
    
    private void TryBroadcastOnExpired()
    {
        if (_hasBroadcastOnExpired)
        {
            return;
        }

        if (!IsExpired)
        {
            throw new InvalidOperationException();
        }

        _hasBroadcastOnExpired = true;
        if (_onExpiredRegistry is not null)
        {
            foreach (var pair in _onExpiredRegistry)
            {
                OnExpiredCallbackRec rec = pair.Value;
                rec.Callback(this, rec.State);
            }
        }
    }

    private void InternalDispose(bool isFromFinalizer)
    {
        if (!_isManaged)
        {
            Logger.Error("Finalize unmanaged export object.");
            return;
        }
        
        if (isFromFinalizer)
        {
            GetOwningAlc().PushPendingDisposeConjugate(this);
            return;
        }
        
        if (Volatile.Read(ref _hasDisposed))
        {
            return;
        }
        
        Volatile.Write(ref _hasDisposed, true);
        
        GetOwningAlc().ReleaseConjugate(Unmanaged);
        MarkAsDead();

        if (!isFromFinalizer)
        {
            GC.SuppressFinalize(this);
        }
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    private void MarkAsDead()
    {
        GCHandle.Free();
        Unmanaged = IConjugate.KDead;
        TryBroadcastOnExpired();
    }

    private readonly record struct OnExpiredCallbackRec(Action<IExplicitLifecycle, object?> Callback, object? State);

    private IntPtr _unmanaged;
    
    private readonly bool _isManaged;
    private bool _hasDisposed;
    
    private uint64 _onExpiredRegistrationHandle;
    private bool _hasBroadcastOnExpired;
    private Dictionary<ExplicitLifecycleExpiredRegistration, OnExpiredCallbackRec>? _onExpiredRegistry;

}


