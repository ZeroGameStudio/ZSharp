// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.CompilerServices;
using System.Threading;

namespace ZeroGames.ZSharp.Core;

internal sealed unsafe class MasterAssemblyLoadContext : ZSharpAssemblyLoadContextBase, IMasterAssemblyLoadContext
{
    
    public static MasterAssemblyLoadContext Create(out bool alreadyExists)
    {
        alreadyExists = Instance is not null;
        return Instance ?? new();
    }

    public static MasterAssemblyLoadContext? Instance { get; private set; }
    
    public Type? GetType(ref readonly RuntimeTypeLocator locator)
    {
        this.GuardUnloaded();
        
        Dictionary<string, Assembly> asmLookup = Assemblies.Concat(Default.Assemblies).ToDictionary(asm => asm.GetName().Name!);

        Func<RuntimeTypeLocator, Type?> combine = null!;
        combine = locator =>
        {
            if (!asmLookup.TryGetValue(locator.AssemblyName, out var asm))
            {
                return null;
            }

            Type? t = asm.GetType(locator.TypeName);
            if (t is null)
            {
                return null;
            }
            
            if (t.IsGenericType)
            {
                if (locator.TypeParameters is null)
                {
                    throw new ArgumentOutOfRangeException();
                }

                Type[] tps = new Type[locator.TypeParameters.Length];
                for (int32 i = 0; i < tps.Length; ++i)
                {
                    Type? tp = combine(locator.TypeParameters[i]);
                    if (tp is null)
                    {
                        return null;
                    }

                    tps[i] = tp;
                }

                return t.MakeGenericType(tps);
            }

            return t;
        };

        return combine(locator);
    }

    public ZCallHandle RegisterZCall(IZCallDispatcher dispatcher)
    {
        GuardInvariant();
        
        ZCallHandle handle = ZCallHandle.Alloc();
        _zcallMap[handle] = dispatcher;
        _zcallName2Handle[dispatcher.Name] = handle;

        return handle;
    }

    public void RegisterZCallResolver(IZCallResolver resolver, uint64 priority)
    {
        GuardInvariant();
        
        _zcallResolverLink.Add((resolver, priority));
        _zcallResolverLink.Sort((lhs, rhs) =>
        {
            if (lhs.Priority == rhs.Priority)
            {
                return 0;
            }

            return lhs.Priority < rhs.Priority ? -1 : 1;
        });
    }

    public EZCallErrorCode ZCall(ZCallHandle handle, ZCallBuffer* buffer)
    {
        GuardInvariant();
        
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

    protected override void HandleUnload()
    {
        try
        {
            if (Instance != this)
            {
                CoreLog.Error("Current Master ALC is not this instance!");
                return;
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
    
    private static void FlushPendingDisposedConjugates(object? state)
    {
        MasterAssemblyLoadContext @this = Unsafe.As<MasterAssemblyLoadContext>(state!);
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

        RegisterZCall(new ZCallDispatcher_Delegate());
        RegisterZCallResolver(new ZCallResolver_Method(this), 1);
        RegisterZCallResolver(new ZCallResolver_Property(this), 2);
    }

    private void GuardInvariant()
    {
        if (!GameThreadScheduler.IsInGameThread)
        {
            throw new InvalidOperationException("Access Master ALC in non-game thread.");
        }
        
        this.GuardUnloaded();
    }
    
    private EZCallErrorCode ZCall_Black(ZCallHandle handle, ZCallBuffer* buffer)
    {
        return (EZCallErrorCode)MasterAssemblyLoadContext_Interop.ZCall_Black(handle, buffer);
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

    private readonly Dictionary<ZCallHandle, IZCallDispatcher> _zcallMap = new();
    private readonly Dictionary<string, ZCallHandle> _zcallName2Handle = new();
    private readonly List<(IZCallResolver Resolver, uint64 Priority)> _zcallResolverLink = new();
    private readonly Dictionary<IntPtr, ConjugateRec> _conjugateMap = new(DEFAULT_CONJUGATE_MAP_CAPACITY);
    private readonly Dictionary<Type, uint16> _conjugateRegistryIdLookup = new();
    private readonly Queue<IConjugate> _pendingDisposedConjugates = new();
    private readonly Lock _pendingDisposedConjugatesLock = new();

    private readonly record struct ConjugateRec(uint16 RegistryId, WeakReference<IConjugate> WeakRef);

}


