// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.UnrealEngine;

public abstract class UnrealConjugateBase : IConjugate
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
    
    public bool IsExpired => Unmanaged == DEAD_ADDR;
    public ReactiveLifecycle Lifecycle => ReactiveLifecycle.Explicit(this);

    public GCHandle GCHandle { get; }

    public IntPtr Unmanaged { get; protected set; }
    public bool IsBlack { get; }
    public bool IsRed => !IsBlack;
    
    internal const IntPtr DEAD_ADDR = 0xDEAD;

    private protected UnrealConjugateBase()
    {
        GCHandle = GCHandle.Alloc(this, GCHandleType.Weak);
        IsBlack = true;
    }

    private protected UnrealConjugateBase(IntPtr unmanaged)
    {
        GCHandle = GCHandle.Alloc(this);
        Unmanaged = unmanaged;
        IsBlack = false;
        
        GC.SuppressFinalize(this);
    }

    ~UnrealConjugateBase() => InternalDispose();
    
    void IConjugate.Release()
    {
        ensure(IsRed);

        MarkAsDead();
    }
    
    private void BroadcastOnExpired()
    {
        ensure(!_hasBroadcastOnExpired);
        
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
        if (!IsBlack)
        {
            throw new InvalidOperationException("Dispose black conjugate.");
        }

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

        if (IsExpired)
        {
            return;
        }
        
        MasterAlcCache.Instance.ReleaseConjugate(this);
        MarkAsDead();
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    private void MarkAsDead()
    {
        ensure(!IsExpired);
        
        GCHandle.Free();
        Unmanaged = DEAD_ADDR;
        BroadcastOnExpired();
    }

    private readonly record struct OnExpiredCallbackRec(Action<IExplicitLifecycle, object?> Callback, object? State);
    
    private bool _disposed;
    
    private uint64 _onExpiredRegistrationHandle;
    private bool _hasBroadcastOnExpired;
    private Dictionary<ExplicitLifecycleExpiredRegistration, OnExpiredCallbackRec>? _onExpiredRegistry;

}


