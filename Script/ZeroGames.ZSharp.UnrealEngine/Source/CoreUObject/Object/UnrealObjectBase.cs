// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

[ConjugateRegistryId(1)]
public abstract class UnrealObjectBase : UnrealConjugateBase, IUnrealFieldPath, IReactiveLifecycleBackend
{
    
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

    public DynamicZCallResult CallUnrealFunctionEx<T>(string name, params ReadOnlySpan<object?> parameters)
    {
        string zcallName = $"uf:/{UnrealFieldPath}:{name}";
        return this.ZCall(MasterAlcCache.Instance, zcallName, [ ..parameters, typeof(T) ]);
    }

    public T? CallUnrealFunction<T>(string name, params ReadOnlySpan<object?> parameters) => (T?)CallUnrealFunctionEx<T>(name, parameters)[-1].Object;

    public DynamicZCallResult CallUnrealFunction(string name, params ReadOnlySpan<object?> parameters)
    {
        string zcallName = $"uf:/{UnrealFieldPath}:{name}";
        return this.ZCall(MasterAlcCache.Instance, zcallName, parameters);
    }

    public DynamicZCallResult ReadUnrealPropertyEx<T>(string name, int32 index)
    {
        string zcallName = $"up:/{UnrealFieldPath}:{name}";
        return this.ZCall(MasterAlcCache.Instance, zcallName, false, index, typeof(T));
    }

    public DynamicZCallResult ReadUnrealPropertyEx<T>(string name) => ReadUnrealPropertyEx<T>(name, 0);

    public T ReadUnrealProperty<T>(string name, int32 index) => (T)ReadUnrealPropertyEx<T>(name, index)[3].Object!;

    public T ReadUnrealProperty<T>(string name) => ReadUnrealProperty<T>(name, 0);

    public DynamicZCallResult WriteUnrealProperty<T>(string name, int32 index, T value)
    {
        string zcallName = $"up:/{UnrealFieldPath}:{name}";
        return this.ZCall(MasterAlcCache.Instance, zcallName, true, index, value);
    }

    public DynamicZCallResult WriteUnrealProperty<T>(string name, T value) => WriteUnrealProperty(name, 0, value);

    public abstract string UnrealFieldPath { get; }
    
    LifecycleToken ILifecycleBackend.Token { get; } = default(LifecycleToken).Next;
    public Lifecycle Lifecycle => ReactiveLifecycle.ForceNonReactive();
    public ReactiveLifecycle ReactiveLifecycle => ReactiveLifecycle.FromBackend(this);
    
    protected UnrealObjectBase(IntPtr unmanaged) : base(unmanaged, true){}

    protected override void HandleExpired()
    {
        base.HandleExpired();
        
        BroadcastOnExpired();
    }

    private readonly record struct OnExpiredCallbackRec(Action? StatelessCallback, Action<object?>? StatefulCallback, object? State);
    
    private void ValidateToken(LifecycleToken token)
    {
        if (token != ((IReactiveLifecycleBackend)this).Token)
        {
            // Throw if token mismatch because this type is not reused and the token should be a constant.
            throw new InvalidOperationException();
        }
    }
    
    private void BroadcastOnExpired()
    {
        verify(!_hasBroadcastOnExpired);
        
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
    
    private uint64 _onExpiredRegistrationHandle;
    private bool _hasBroadcastOnExpired;
    private Dictionary<LifecycleExpiredRegistration, OnExpiredCallbackRec>? _onExpiredRegistry;

}


