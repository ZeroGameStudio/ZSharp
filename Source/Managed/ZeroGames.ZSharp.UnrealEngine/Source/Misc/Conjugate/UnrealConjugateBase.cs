// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using InvalidOperationException = System.InvalidOperationException;

namespace ZeroGames.ZSharp.UnrealEngine;

public abstract class UnrealConjugateBase : IConjugate, IReactiveUnderlyingLifecycle
{

    public void Dispose()
    {
        InternalDispose();
        GC.SuppressFinalize(this);
    }

    public LifecycleExpiredRegistration RegisterOnExpired(Action callback)
    {
        Thrower.ThrowIfNotInGameThread();
        
        if (IsExpired)
        {
            callback();

            return default;
        }
        else
        {
            _onExpiredRegistry ??= new();
            LifecycleExpiredRegistration reg = new(Lifecycle, ++_onExpiredRegistrationHandle);
            _onExpiredRegistry[reg] = new(callback, null, null);

            return reg;
        }
    }
    LifecycleExpiredRegistration IReactiveUnderlyingLifecycle.RegisterOnExpired(Action callback, UnderlyingLifecycleToken token)
    {
        ValidateToken(token);
        return RegisterOnExpired(callback);
    }

    public LifecycleExpiredRegistration RegisterOnExpired(Action<object?> callback, object? state)
    {
        Thrower.ThrowIfNotInGameThread();
        
        if (IsExpired)
        {
            callback(state);

            return default;
        }
        else
        {
            _onExpiredRegistry ??= new();
            LifecycleExpiredRegistration reg = new(Lifecycle, ++_onExpiredRegistrationHandle);
            _onExpiredRegistry[reg] = new(null, callback, state);

            return reg;
        }
    }
    LifecycleExpiredRegistration IReactiveUnderlyingLifecycle.RegisterOnExpired(Action<object?> callback, object? state, UnderlyingLifecycleToken token)
    {
        ValidateToken(token);
        return RegisterOnExpired(callback, state);
    }

    bool IUnderlyingLifecycle.IsExpired(UnderlyingLifecycleToken token)
    {
        ValidateToken(token);
        return IsExpired;
    }
    
    public bool IsExpired => Unmanaged == DEAD_ADDR;
    public ReactiveLifecycle Lifecycle => ReactiveLifecycle.FromUnderlyingLifecycle(this);

    public GCHandle GCHandle { get; }

    public IntPtr Unmanaged { get; protected set; }
    public bool IsBlack { get; }
    public bool IsRed => !IsBlack;

    UnderlyingLifecycleToken IUnderlyingLifecycle.Token { get; } = default(UnderlyingLifecycleToken).Next;
    
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
                    rec.StatelessCallback?.Invoke();
                    rec.StatefulCallback?.Invoke(rec.State);
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

    private void ValidateToken(UnderlyingLifecycleToken token)
    {
        if (token != ((IReactiveUnderlyingLifecycle)this).Token)
        {
            throw new InvalidOperationException();
        }
    }

    private readonly record struct OnExpiredCallbackRec(Action? StatelessCallback, Action<object?>? StatefulCallback, object? State);
    
    private bool _disposed;

    private uint64 _onExpiredRegistrationHandle;
    private bool _hasBroadcastOnExpired;
    private Dictionary<LifecycleExpiredRegistration, OnExpiredCallbackRec>? _onExpiredRegistry;
    
}


