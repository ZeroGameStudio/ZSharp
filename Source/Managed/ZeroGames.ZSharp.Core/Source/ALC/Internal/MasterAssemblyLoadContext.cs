// Copyright Zero Games. All Rights Reserved.

using System.Collections.Concurrent;
using System.Reflection;
using System.Threading;

namespace ZeroGames.ZSharp.Core;

internal sealed unsafe class MasterAssemblyLoadContext : ZSharpAssemblyLoadContextBase, IMasterAssemblyLoadContext
{
    
    public static MasterAssemblyLoadContext Create(out bool alreadyExists)
    {
        alreadyExists = Instance is not null;
        return Instance ?? new();
    }

    public static MasterAlcUnloadingRegistration RegisterUnloading(Action callback, int64 priority)
    {
        MasterAlcUnloadingRegistration registration = new(++_unloadingHandle);
        _unloadingCallbacks.Add(new(registration, callback, priority));
        _unloadingCallbacks.Sort(static (lhs, rhs) => Math.Sign(lhs.Priority - rhs.Priority));
        return registration;
    }

    public static void UnregisterUnloading(MasterAlcUnloadingRegistration registration)
    {
        for (int32 i = 0; i < _unloadingCallbacks.Count; ++i)
        {
            if (registration == _unloadingCallbacks[i].Registration)
            {
                _unloadingCallbacks.RemoveAt(i);
                return;
            }
        }
    }

    public Type? GetType(RuntimeTypeUri uri)
    {
        this.GuardUnloaded();

        if (string.IsNullOrWhiteSpace(uri.Uri))
        {
            return null;
        }

        return GetTypeForCompositeKey(uri.Uri);
    }

    public ZCallHandle RegisterZCall(IZCallDispatcher dispatcher)
    {
        GuardInvariant();
        
        ZCallHandle handle = ZCallHandle.Alloc();
        _zcallMap[handle] = dispatcher;
        _zcallName2Handle[dispatcher.Name] = handle;

        return handle;
    }

    public void RegisterZCallResolver(IZCallResolver resolver, int64 priority)
    {
        GuardInvariant();
        
        _zcallResolverLink.Add((resolver, priority));
        _zcallResolverLink.Sort((lhs, rhs) => Math.Sign(lhs.Priority - rhs.Priority));
    }

    public EZCallErrorCode ZCall(ZCallHandle handle, ZCallBuffer* buffer)
    {
        GuardInvariant();

        if (!handle.IsBlack)
        {
            return EZCallErrorCode.InvalidHandle;
        }
        
        return ZCall_Black(handle, buffer);
    }

    public ZCallHandle GetZCallHandle(string name)
    {
        GuardInvariant();
        
        return GetZCallHandle_Black(name);
    }

    public IntPtr BuildConjugate(IConjugate managed, IntPtr userdata)
    {
        GuardInvariant();
        
        return BuildConjugate_Black(managed, userdata);
    }

    public void ReleaseConjugate(IConjugate conjugate)
    {
        GuardInvariant();
        
        ReleaseConjugate_Black(conjugate.Unmanaged);
    }
    
    public void PushPendingDisposeConjugate(IConjugate conjugate)
    {
        this.GuardUnloaded();

        if (GameThreadScheduler.IsInGameThread)
        {
            conjugate.Dispose();
        }
        else
        {
            lock (_pendingDisposedConjugatesLock)
            {
                _pendingDisposedConjugates.Enqueue(conjugate);
                GameThreadScheduler.Post(FlushPendingDisposedConjugates, this);
            }
        }
    }

    public EZCallErrorCode ZCall_Red(ZCallHandle handle, ZCallBuffer* buffer)
    {
        GuardInvariant();
        
        if (!_zcallMap.TryGetValue(handle, out var dispatcher))
        {
            return EZCallErrorCode.DispatcherNotFound;
        }

        return dispatcher.Dispatch(buffer);
    }

    public ZCallHandle GetZCallHandle_Red(string name)
    {
        GuardInvariant();
        
        if (!_zcallName2Handle.TryGetValue(name, out var handle))
        {
            foreach (var pair in _zcallResolverLink)
            {
                IZCallDispatcher? dispatcher = pair.Resolver.Resolve(name);
                if (dispatcher is not null)
                {
                    handle = RegisterZCall(dispatcher);
                    break;
                }
            }
        }

        return handle;
    }

    public IntPtr BuildConjugate_Red(IntPtr unmanaged, Type type)
    {
        GuardInvariant();
        
        Type conjugateType = typeof(IConjugate<>).MakeGenericType(type);
        if (type.IsAssignableTo(conjugateType))
        {
            IConjugate conjugate = (IConjugate)type.GetMethod("BuildConjugate")!.Invoke(null, new object[] { unmanaged })!;
            _conjugateMap[unmanaged] = new(0, new(conjugate, true));
            return unmanaged;
        }

        return new();
    }

    public void ReleaseConjugate_Red(IntPtr unmanaged)
    {
        GuardInvariant();

        if (!_conjugateMap.Remove(unmanaged, out var rec))
        {
            CoreLog.Error($"Conjugate {unmanaged} doesn't exist!");
            return;
        }

        if (!rec.WeakRef.TryGetTarget(out var conjugate))
        {
            CoreLog.Error($"Conjugate {unmanaged} is expired!");
            return;
        }

        conjugate.Release();
    }

    public IConjugate? Conjugate(IntPtr unmanaged)
    {
        GuardInvariant();
        
        if (!_conjugateMap.TryGetValue(unmanaged, out var rec))
        {
            return null;
        }

        if (!rec.WeakRef.TryGetTarget(out var conjugate))
        {
            CoreLog.Error($"Conjugate {unmanaged} is expired!");
            return null;
        }
        
        return conjugate;
    }
    
    public const string INSTANCE_NAME = "Master";
    
    public static MasterAssemblyLoadContext? Instance { get; private set; }

    protected override void HandleAssemblyLoaded(Assembly assembly)
    {
        base.HandleAssemblyLoaded(assembly);

        foreach (var type in assembly.GetTypes())
        {
            if (!type.IsClass)
            {
                continue;
            }

            if (type.GetCustomAttribute<ConjugateKeyAttribute>() is { } attr)
            {
                string key = attr.Key;
                if (!_conjugateKeyCache.TryAdd(key, type))
                {
                    CoreLog.Error($"Failed to cache conjugate key [{key}] with type [{type.FullName}] because it already exists [{_conjugateKeyCache[key].FullName}].");
                }
            }
        }
    }

    protected override void HandleUnload()
    {
        try
        {
            if (Instance != this)
            {
                CoreLog.Error("Current Master ALC is not this instance!");
                return;
            }

            try
            {
                foreach (var rec in _unloadingCallbacks)
                {
                    rec.Callback();
                }
            }
            catch (Exception ex)
            {
                CoreLog.Warning($"Unhandled exception detected in MasterALC unloading callback.\n{ex}");
            }

            // Dispose all conjugates to ensure that there is no resource leak.
            Dictionary<IntPtr, ConjugateRec> temp = _conjugateMap.ToDictionary();
            foreach (var pair in temp)
            {
                if (!pair.Value.WeakRef.TryGetTarget(out var conjugate))
                {
                    CoreLog.Error($"Conjugate {pair.Key} is expired!");
                    continue;
                }
                
                // All red conjugates should have been released at this point.
                if (!conjugate.IsBlack)
                {
                    CoreLog.Error($"Conjugate {pair.Key} is not black!");
                    continue;
                }
                
                conjugate.Dispose();
            }

            // Manually flush pending disposed conjugates before mark IsUnloaded.
            FlushPendingDisposedConjugates(this);

            Instance = null;
        }
        finally
        {
            base.HandleUnload();
        }
    }
    
    private readonly record struct ConjugateRec(uint16 RegistryId, WeakReference<IConjugate> WeakRef);
    private readonly record struct UnloadingRec(MasterAlcUnloadingRegistration Registration, Action Callback, int64 Priority);
    
    private static void FlushPendingDisposedConjugates(object? state)
    {
        MasterAssemblyLoadContext @this = (MasterAssemblyLoadContext)state!;
        lock (@this._pendingDisposedConjugatesLock)
        {
            if (@this.IsUnloaded)
            {
                if (@this._pendingDisposedConjugates.Count != 0)
                {
                    CoreLog.Error("Master ALC is unloaded but conjugate map is not empty!");
                }
                return;
            }

            while (@this._pendingDisposedConjugates.TryDequeue(out var conjugate))
            {
                conjugate.Dispose();
            }
        }
    }

    private MasterAssemblyLoadContext() : base(INSTANCE_NAME)
    {
        Instance = this;

        _conjugateKeyCache.TryAdd($"@{typeof(uint8).FullName}", typeof(uint8));
        _conjugateKeyCache.TryAdd($"@{typeof(uint16).FullName}", typeof(uint16));
        _conjugateKeyCache.TryAdd($"@{typeof(uint32).FullName}", typeof(uint32));
        _conjugateKeyCache.TryAdd($"@{typeof(uint64).FullName}", typeof(uint64));
        _conjugateKeyCache.TryAdd($"@{typeof(int8).FullName}", typeof(int8));
        _conjugateKeyCache.TryAdd($"@{typeof(int16).FullName}", typeof(int16));
        _conjugateKeyCache.TryAdd($"@{typeof(int32).FullName}", typeof(int32));
        _conjugateKeyCache.TryAdd($"@{typeof(int64).FullName}", typeof(int64));
        _conjugateKeyCache.TryAdd($"@{typeof(float).FullName}", typeof(float));
        _conjugateKeyCache.TryAdd($"@{typeof(double).FullName}", typeof(double));
        _conjugateKeyCache.TryAdd($"@{typeof(bool).FullName}", typeof(bool));

        RegisterZCall(new ZCallDispatcher_Delegate());
        RegisterZCallResolver(new ZCallResolver_Method(this), 1);
        RegisterZCallResolver(new ZCallResolver_Property(this), 2);
    }

    private void GuardInvariant()
    {
        Thrower.ThrowIfNotInGameThread("Access Master ALC in non-game thread.");
        this.GuardUnloaded();
    }
    
    private Type? GetTypeForCompositeKey(string key)
    {
        int32 quoteIndex = key.IndexOf('<');
        // Early out if this is a non-generic type.
        if (quoteIndex == -1)
        {
            return GetTypeForSingleKey(key);
        }
        
        // Recursively fill generic arguments.
        string rootKey = key.Substring(0, quoteIndex);
        string[] remainingKeys = key.Substring(rootKey.Length + 1, key.Length - rootKey.Length - 2).Split(',');
        Type? type = GetTypeForSingleKey(rootKey);
        if (type is null)
        {
            return null;
        }
        
        if (remainingKeys.Length == 0 || !type.IsGenericType)
        {
            return null;
        }
        
        Type[] genericArguments = new Type[remainingKeys.Length];
        for (int32 i = 0; i < genericArguments.Length; ++i)
        {
            Type? innerType = GetTypeForCompositeKey(remainingKeys[i]);
            if (innerType is null)
            {
                return null;
            }
            genericArguments[i] = innerType;
        }
        
        return type.MakeGenericType(genericArguments);
    }

    private Type? GetTypeForSingleKey(string key)
    {
        _conjugateKeyCache.TryGetValue(key, out var type);
        return type;
    }
    
    private EZCallErrorCode ZCall_Black(ZCallHandle handle, ZCallBuffer* buffer)
    {
        return MasterAssemblyLoadContext_Interop.ZCall_Black(handle, buffer);
    }

    private ZCallHandle GetZCallHandle_Black(string name)
    {
        fixed (char* data = name)
        {
            return MasterAssemblyLoadContext_Interop.GetZCallHandle_Black(data);
        }
    }

    private IntPtr BuildConjugate_Black(IConjugate managed, IntPtr userdata)
    {
        uint16 registryId = GetTypeConjugateRegistryId(managed.GetType());
        if (registryId == 0)
        {
            CoreLog.Error($"Type {managed.GetType().FullName} does not have a valid conjugate registry id.");
            return default;
        }

        IntPtr unmanaged = MasterAssemblyLoadContext_Interop.BuildConjugate_Black(registryId, userdata);
        if (unmanaged == default)
        {
            CoreLog.Error($"Failed to build conjugate for type {managed.GetType().FullName}");
            return default;
        }
        
        _conjugateMap[unmanaged] = new(registryId, new(managed, true));

        return unmanaged;
    }

    private void ReleaseConjugate_Black(IntPtr unmanaged)
    {
        if (!_conjugateMap.Remove(unmanaged, out var rec))
        {
            CoreLog.Error($"Conjugate {unmanaged} doesn't exist!");
            return;
        }

        MasterAssemblyLoadContext_Interop.ReleaseConjugate_Black(rec.RegistryId, unmanaged);
    }

    private uint16 GetTypeConjugateRegistryId(Type type)
    {
        if (_conjugateRegistryIdLookup.TryGetValue(type, out var cachedId))
        {
            return cachedId;
        }
        
        if (type.GetCustomAttribute<ConjugateRegistryIdAttribute>()?.Id is {} id)
        {
            _conjugateRegistryIdLookup[type] = id;
            return id;
        }

        return 0;
    }

    private const int32 DEFAULT_CONJUGATE_MAP_CAPACITY = 1 << 16;

    private static readonly List<UnloadingRec> _unloadingCallbacks = new();
    private static uint64 _unloadingHandle;

    private readonly ConcurrentDictionary<string, Type> _conjugateKeyCache = new();
    private readonly Dictionary<ZCallHandle, IZCallDispatcher> _zcallMap = new();
    private readonly Dictionary<string, ZCallHandle> _zcallName2Handle = new();
    private readonly List<(IZCallResolver Resolver, int64 Priority)> _zcallResolverLink = new();
    private readonly Dictionary<IntPtr, ConjugateRec> _conjugateMap = new(DEFAULT_CONJUGATE_MAP_CAPACITY);
    private readonly Dictionary<Type, uint16> _conjugateRegistryIdLookup = new();
    private readonly Queue<IConjugate> _pendingDisposedConjugates = new();
    private readonly Lock _pendingDisposedConjugatesLock = new();

}


