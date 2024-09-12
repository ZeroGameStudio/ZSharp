// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Runtime.Loader;
using System.Threading;

namespace ZeroGames.ZSharp.UnrealEngine;

/**
 * Red Lifecycle:
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
 * Black Lifecycle:
 *     AllocManagedObject (request by managed user code)
 *     -> AllocUnmanagedResource & BuildConjugate (request by managed code)
 *     -> ...
 *     -> Dispose or Finalize
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
            if (alc != IMasterAssemblyLoadContext.Instance)
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
        if (IsBlack)
        {
            Logger.Error("Dispose black conjugate from unmanaged code.");
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
        if (IsExpired)
        {
            callback(this, state);

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

    public void UnregisterOnExpired(ExplicitLifecycleExpiredRegistration registration)
    {
        if (!IsExpired)
        {
            _onExpiredRegistry?.Remove(registration);
        }
    }

    public bool IsValidRegistration(ExplicitLifecycleExpiredRegistration registration)
    {
        if (IsExpired)
        {
            return false;
        }

        return _onExpiredRegistry?.ContainsKey(registration) ?? false;
    }

    public GCHandle GCHandle { get; }

    public IntPtr Unmanaged { get; protected set; }
    public bool IsBlack { get; }

    public bool IsExpired => Unmanaged == IConjugate.KDead;

    public bool IsGameThreadOnly => true;
    public object SyncRoot => this;

    private protected ExportedObjectBase()
    {
        GCHandle = GCHandle.Alloc(this, GCHandleType.Weak);
        IsBlack = true;
    }

    private protected ExportedObjectBase(IntPtr unmanaged)
    {
        GCHandle = GCHandle.Alloc(this);
        Unmanaged = unmanaged;
        IsBlack = false;
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
                try
                {
                    rec.Callback(this, rec.State);
                }
                catch (Exception ex)
                {
                    Logger.Error($"Unhandled Exception Detected.\n{ex}");
                }

                _onExpiredRegistry = null;
            }
        }
    }

    private void InternalDispose(bool fromFinalizer)
    {
        if (!IsBlack)
        {
            Logger.Error("Finalize unmanaged export object.");
            return;
        }
        
        if (fromFinalizer)
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
        TryBroadcastOnExpired();
    }

    private readonly record struct OnExpiredCallbackRec(Action<IExplicitLifecycle, object?> Callback, object? State);
    
    private bool _hasDisposed;
    
    private uint64 _onExpiredRegistrationHandle;
    private bool _hasBroadcastOnExpired;
    private Dictionary<ExplicitLifecycleExpiredRegistration, OnExpiredCallbackRec>? _onExpiredRegistry;

}


