// Copyright Zero Games. All Rights Reserved.

using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using InvalidOperationException = System.InvalidOperationException;

namespace ZeroGames.ZSharp.UnrealEngine;

public abstract class UnrealConjugateBase : IConjugate, IReactiveLifecycleBackend
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
            LifecycleExpiredRegistration reg = new(this, ++_onExpiredRegistrationHandle);
            _onExpiredRegistry[reg] = new(callback, null, null);

            return reg;
        }
    }
    LifecycleExpiredRegistration IReactiveLifecycleBackend.RegisterOnExpired(Action callback, LifecycleToken token)
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
            LifecycleExpiredRegistration reg = new(this, ++_onExpiredRegistrationHandle);
            _onExpiredRegistry[reg] = new(null, callback, state);

            return reg;
        }
    }
    LifecycleExpiredRegistration IReactiveLifecycleBackend.RegisterOnExpired(Action<object?> callback, object? state, LifecycleToken token)
    {
        ValidateToken(token);
        return RegisterOnExpired(callback, state);
    }
    
    public void UnregisterOnExpired(LifecycleExpiredRegistration registration)
    {
        Thrower.ThrowIfNotInGameThread();
        
        if (!IsExpired)
        {
            _onExpiredRegistry?.Remove(registration);
        }
    }
    void IReactiveLifecycleBackend.UnregisterOnExpired(LifecycleExpiredRegistration registration, LifecycleToken token)
    {
        ValidateToken(token);
        UnregisterOnExpired(registration);
    }

    bool ILifecycleBackend.IsExpired(LifecycleToken token)
    {
        ValidateToken(token);
        return IsExpired;
    }
    
    public bool IsExpired => Unmanaged == DEAD_ADDR;
    public Lifecycle Lifecycle => ReactiveLifecycle.ForceNonReactive();
    public ReactiveLifecycle ReactiveLifecycle => ReactiveLifecycle.FromBackend(this);

    public GCHandle GCHandle { get; }

    public IntPtr Unmanaged { get; private set; }
    public bool IsBlack { get; }
    public bool IsRed => !IsBlack;

    LifecycleToken ILifecycleBackend.Token { get; } = default(LifecycleToken).Next;
    
    internal const IntPtr DEAD_ADDR = 0xDEAD;

    protected void BuildConjugate_Black(IntPtr userdata)
    {
        if (Unmanaged != IntPtr.Zero)
        {
            throw new InvalidOperationException();
        }
        
        Unmanaged = MasterAlcCache.Instance.BuildConjugate(this, userdata);
    }
    
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
    
    private void ValidateToken(LifecycleToken token)
    {
        if (token != ((IReactiveLifecycleBackend)this).Token)
        {
            // Throw if token mismatch because this type is not reused and the token should be a constant.
            throw new InvalidOperationException();
        }
    }

    private readonly record struct OnExpiredCallbackRec(Action? StatelessCallback, Action<object?>? StatefulCallback, object? State);
    
    private bool _disposed;

    private uint64 _onExpiredRegistrationHandle;
    private bool _hasBroadcastOnExpired;
    private Dictionary<LifecycleExpiredRegistration, OnExpiredCallbackRec>? _onExpiredRegistry;
    
}


