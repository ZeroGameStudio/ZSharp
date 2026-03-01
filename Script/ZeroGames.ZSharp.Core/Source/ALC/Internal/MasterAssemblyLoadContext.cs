// Copyright Zero Games. All Rights Reserved.

using System.Collections.Concurrent;
using System.Diagnostics;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
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

        bool lockTaken = false;
        try
        {
            _pendingDisposedConjugatesLock.Enter(ref lockTaken);
            
            _pendingDisposedConjugates.Enqueue(conjugate);
            ConditionallyPostFlushPendingDisposedConjugates();
        }
        finally
        {
            if (lockTaken)
            {
                _pendingDisposedConjugatesLock.Exit();
            }
        }
    }

    public void PrepareUnloading()
    {
        if (_unloadingPrepared)
        {
            CoreLog.Error("PrepareUnloading() called more than once!");
            return;
        }
        
        _unloadingPrepared = true;
        
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
            // This happens when a black conjugate was pushed into pending disposed queue (resurrected) but the queue is not flushed yet.
            // Just skip here because we are going to flush the queue the next step.
            if (pair.Value.Handle.Target is not { } conjugate)
            {
                continue;
            }
            
            // Black only, unmanaged side will handle red.
            if (conjugate.IsBlack)
            {
                conjugate.Dispose();
            }
        }
        
        FlushPendingDisposedConjugates(true);
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

    public IntPtr BuildConjugate_Red(IntPtr unmanaged, IntPtr typeHandle)
    {
        GuardInvariant();
        GuardUnloadingPrepared();
        
        if (!_buildRedConjugateDelegateLookup.TryGetValue(typeHandle, out var buildConjugate))
        {
            Type? type = Type.GetTypeFromHandle(RuntimeTypeHandle.FromIntPtr(typeHandle));
            Thrower.FatalIf(type is null);
            MethodInfo? method = type.GetMethod(BUILD_CONJUGATE_METHOD_NAME);
            Thrower.FatalIf(method is null);
            buildConjugate = method.CreateDelegate<Func<IntPtr, IConjugate>>();
            _buildRedConjugateDelegateLookup[typeHandle] = buildConjugate;
        }
        
        IConjugate conjugate = buildConjugate(unmanaged);
        _conjugateMap[unmanaged] = new(0, BuildConjugateHandle(conjugate));
        return unmanaged;
    }

    public void ReleaseConjugate_Red(IntPtr unmanaged)
    {
        GuardInvariant();

        if (!_conjugateMap.Remove(unmanaged, out var rec))
        {
            CoreLog.Error($"Conjugate {unmanaged} doesn't exist!");
            return;
        }

        if (rec.Handle.Target is not { } conjugate)
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

        if (rec.Handle.Target is not { } conjugate)
        {
            CoreLog.Error($"Conjugate {unmanaged} is expired!");
            return null;
        }
        
        return conjugate;
    }
    
    public string StatBlackConjugates(string typeName)
    {
        if (!_statBlackConjugates.TryGetValue(typeName, out var stat))
        {
            return string.Empty;
        }
        
        return string.Join("&&&&&&&", stat.OrderByDescending(pair => pair.Value).Select(pair => $"{pair.Key}|||||||{pair.Value}"));
    }
    
    public const string INSTANCE_NAME = "Master";
    
    public static MasterAssemblyLoadContext? Instance { get; private set; }

    public uint64 Generation { get; }

    protected override void HandleUnload()
    {
        try
        {
            if (Instance != this)
            {
                CoreLog.Error("Current Master ALC is not this instance!");
                return;
            }

            Instance = null;
        }
        finally
        {
            base.HandleUnload();
        }
    }

    protected override void HandleAssemblyLoaded(Assembly assembly)
    {
        foreach (var type in assembly.GetTypes())
        {
            if (type is { IsClass: false, IsInterface: false })
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
        
        base.HandleAssemblyLoaded(assembly);
    }
    
    private readonly record struct ConjugateRec(uint16 RegistryId, GCHandle<IConjugate?> Handle);
    private readonly record struct UnloadingRec(MasterAlcUnloadingRegistration Registration, Action Callback, int64 Priority);

    private MasterAssemblyLoadContext() : base(INSTANCE_NAME)
    {
        ++_generation;
        Generation = _generation;
        
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
    }

    [Conditional("ASSERTION_CHECK_SLOW")]
    private void GuardInvariant()
    {
        Thrower.ThrowIfNotInGameThread("Access Master ALC in non-game thread.");
        this.GuardUnloaded();
    }
    
    [Conditional("ASSERTION_CHECK_SLOW")]
    private void GuardUnloadingPrepared()
    {
        if (_unloadingPrepared)
        {
            throw new InvalidOperationException("Master ALC is Unloading.");
        }
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

    private GCHandle<IConjugate?> BuildConjugateHandle(IConjugate managed)
        => GCHandle<IConjugate?>.FromIntPtr(GCHandle.ToIntPtr(GCHandle.Alloc(managed, GCHandleType.Weak)));

    private IntPtr BuildConjugate_Black(IConjugate managed, IntPtr userdata)
    {
        GuardUnloadingPrepared();

        Type type = managed.GetType();
        uint16 registryId = GetTypeConjugateRegistryId(type);
        if (registryId == 0)
        {
            CoreLog.Error($"Type {type.FullName} does not have a valid conjugate registry id.");
            return IntPtr.Zero;
        }

        IntPtr unmanaged = MasterAssemblyLoadContext_Interop.BuildConjugate_Black(registryId, userdata);
        if (unmanaged == IntPtr.Zero)
        {
            CoreLog.Error($"Failed to build conjugate for type {type.FullName}");
            return IntPtr.Zero;
        }
        
        _conjugateMap[unmanaged] = new(registryId, BuildConjugateHandle(managed));
        
        #if STAT_BLACK_CONJUGATES
        
        string stackTrace = new StackTrace().ToString();
        StatBlackConjugate(type, stackTrace);
        
        #endif

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
    
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    private void ConditionallyPostFlushPendingDisposedConjugates()
    {
        if (_pendingDisposedConjugates.Count > 0 && !_flushPendingDisposedConjugatesPosted)
        {
            GameThreadScheduler.Post(static state => Unsafe.As<MasterAssemblyLoadContext>(state)!.FlushPendingDisposedConjugates(false), this);
            _flushPendingDisposedConjugatesPosted = true;
        }
    }
    
    private void FlushPendingDisposedConjugates(bool forceFullDispose)
    {
        bool lockTaken = false;
        try
        {
            _pendingDisposedConjugatesLock.Enter(ref lockTaken);

            _flushPendingDisposedConjugatesPosted = false;
            
            if (IsUnloaded)
            {
                if (_pendingDisposedConjugates.Count is not 0)
                {
                    CoreLog.Error("Master ALC is unloaded but conjugate map is not empty!");
                }
                return;
            }

            if (!forceFullDispose)
            {
                const double HARD_DISPOSE_FACTOR = 0.05;
                const double TIME_BUDGET_MS = 1;
                
                _flushPendingDisposedConjugatesStopwatch.Restart();
                
                int32 hardDisposeCount = (int32)(_pendingDisposedConjugates.Count * HARD_DISPOSE_FACTOR);
                int32 disposedCount = 0;
                while (_pendingDisposedConjugates.TryDequeue(out var conjugate))
                {
                    conjugate.Dispose();
                    
                    if (++disposedCount >= hardDisposeCount && _flushPendingDisposedConjugatesStopwatch.Elapsed.TotalMilliseconds > TIME_BUDGET_MS)
                    {
                        ConditionallyPostFlushPendingDisposedConjugates();
                        break;
                    }
                }
            }
            else
            {
                while (_pendingDisposedConjugates.TryDequeue(out var conjugate))
                {
                    conjugate.Dispose();
                }
            }
        }
        finally
        {
            if (lockTaken)
            {
                _pendingDisposedConjugatesLock.Exit();
            }
        }
    }
    
    private void StatBlackConjugate(Type type, string stackTrace)
    {
        string typeName = type.Name;
        if (!_statBlackConjugates.TryGetValue(typeName, out var stat))
        {
            stat = [];
            _statBlackConjugates[typeName] = stat;
        }
        
        if (stat.TryGetValue(stackTrace, out var count))
        {
            stat[stackTrace] = count + 1;
        }
        else
        {
            stat[stackTrace] = 1;
        }
    }

    private const int32 DEFAULT_CONJUGATE_MAP_CAPACITY = 1 << 16;
    private const string BUILD_CONJUGATE_METHOD_NAME = nameof(IConjugate<>.BuildConjugate);

    private static uint64 _generation;

    private static readonly List<UnloadingRec> _unloadingCallbacks = new();
    private static uint64 _unloadingHandle;

    private readonly ConcurrentDictionary<string, Type> _conjugateKeyCache = new();
    private readonly Dictionary<ZCallHandle, IZCallDispatcher> _zcallMap = new();
    private readonly Dictionary<string, ZCallHandle> _zcallName2Handle = new();
    private readonly List<(IZCallResolver Resolver, int64 Priority)> _zcallResolverLink = new();
    private readonly Dictionary<IntPtr, ConjugateRec> _conjugateMap = new(DEFAULT_CONJUGATE_MAP_CAPACITY);
    private readonly Dictionary<Type, uint16> _conjugateRegistryIdLookup = new();
    private readonly Queue<IConjugate> _pendingDisposedConjugates = new();
    private SpinLock _pendingDisposedConjugatesLock;
    private bool _flushPendingDisposedConjugatesPosted;
    private readonly Stopwatch _flushPendingDisposedConjugatesStopwatch = new();
    
    private bool _unloadingPrepared;

    private readonly Dictionary<IntPtr, Func<IntPtr, IConjugate>> _buildRedConjugateDelegateLookup = [];

    private readonly Dictionary<string, Dictionary<string, uint64>> _statBlackConjugates = [];

}


