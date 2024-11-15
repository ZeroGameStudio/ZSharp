// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

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

    public void Dispose()
    {
        InternalDispose();
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

    public bool IsExpired => Unmanaged == DEAD_ADDR;

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

    ~ExportedObjectBase() => InternalDispose();
    
    void IConjugate.Release()
    {
        check(!IsBlack);
        check(!IsExpired);
        
        MarkAsDead();
        
        GC.SuppressFinalize(this);
    }
    
    private void BroadcastOnExpired()
    {
        check(!_hasBroadcastOnExpired);
        
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
                    UnhandledExceptionHelper.Guard(ex, null, LogZSharpScriptEngine);
                }

                _onExpiredRegistry = null;
            }
        }
    }

    private void InternalDispose()
    {
        check(IsBlack);

        if (!IsInGameThread)
        {
            MasterAlcCache.Instance.PushPendingDisposeConjugate(this);
            return;
        }
        
        if (_disposed)
        {
            return;
        }

        _disposed = true;
        
        MasterAlcCache.Instance.ReleaseConjugate(this);
        MarkAsDead();
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    private void MarkAsDead()
    {
        check(!IsExpired);
        
        GCHandle.Free();
        Unmanaged = DEAD_ADDR;
        BroadcastOnExpired();
    }

    private readonly record struct OnExpiredCallbackRec(Action<IExplicitLifecycle, object?> Callback, object? State);
    
    private const IntPtr DEAD_ADDR = 0xDEAD;
    
    private bool _disposed;
    
    private uint64 _onExpiredRegistrationHandle;
    private bool _hasBroadcastOnExpired;
    private Dictionary<ExplicitLifecycleExpiredRegistration, OnExpiredCallbackRec>? _onExpiredRegistry;

}


