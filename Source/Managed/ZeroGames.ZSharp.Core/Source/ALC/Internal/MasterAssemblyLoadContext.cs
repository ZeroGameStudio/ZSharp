// Copyright Zero Games. All Rights Reserved.

using System.Collections.Concurrent;
using System.Reflection;

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
                check(locator.TypeParameters is not null);

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
        check(IsInGameThread);
        this.GuardUnloaded();
        
        ZCallHandle handle = ZCallHandle.Alloc();
        _zcallMap[handle] = dispatcher;
        _zcallName2Handle[dispatcher.Name] = handle;

        return handle;
    }

    public void RegisterZCallResolver(IZCallResolver resolver, uint64 priority)
    {
        check(IsInGameThread);
        this.GuardUnloaded();
        
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
        check(IsInGameThread);
        this.GuardUnloaded();
        
        return ZCall_Black(handle, buffer);
    }

    public ZCallHandle GetZCallHandle(string name)
    {
        check(IsInGameThread);
        this.GuardUnloaded();
        
        return GetZCallHandle_Black(name);
    }

    public IntPtr BuildConjugate(IConjugate managed, IntPtr userdata)
    {
        check(IsInGameThread);
        this.GuardUnloaded();
        
        return BuildConjugate_Black(managed, userdata);
    }

    public void ReleaseConjugate(IConjugate conjugate)
    {
        check(IsInGameThread);
        this.GuardUnloaded();
        
        ReleaseConjugate_Black(conjugate.Unmanaged);
    }
    
    public void PushPendingDisposeConjugate(IConjugate conjugate)
    {
        this.GuardUnloaded();

        if (IsInGameThread)
        {
            conjugate.Dispose();
        }
        else
        {
            _pendingDisposeConjugates.Enqueue(conjugate);
        }
    }

    public void Tick(float deltaTime)
    {
        check(IsInGameThread);
        this.GuardUnloaded();

        while (_pendingDisposeConjugates.TryDequeue(out var conjugate))
        {
            conjugate.Dispose();
        }
    }
    
    public EZCallErrorCode ZCall_Red(ZCallHandle handle, ZCallBuffer* buffer)
    {
        this.GuardUnloaded();
        
        if (!_zcallMap.TryGetValue(handle, out var dispatcher))
        {
            return EZCallErrorCode.DispatcherNotFound;
        }

        return dispatcher.Dispatch(buffer);
    }

    public ZCallHandle GetZCallHandle_Red(string name)
    {
        check(IsInGameThread);
        this.GuardUnloaded();
        
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
        this.GuardUnloaded();
        
        Type conjugateType = typeof(IConjugate<>).MakeGenericType(type);
        if (type.IsAssignableTo(conjugateType))
        {
            IConjugate conjugate = (IConjugate)type.GetMethod("BuildConjugate")!.Invoke(null, new object[] { unmanaged })!;
            _conjugateMap[unmanaged] = new(0, new(conjugate));
            return unmanaged;
        }

        return new();
    }

    public void ReleaseConjugate_Red(IntPtr unmanaged)
    {
        this.GuardUnloaded();
        
        verify(_conjugateMap.Remove(unmanaged, out var rec));
        verify(rec.WeakRef.TryGetTarget(out var conjugate));
        conjugate.Release();
    }

    public IConjugate? Conjugate(IntPtr unmanaged)
    {
        this.GuardUnloaded();
        
        if (!_conjugateMap.TryGetValue(unmanaged, out var rec))
        {
            return null;
        }

        // Recorded conjugate should always be alive.
        verify(rec.WeakRef.TryGetTarget(out var conjugate));
        return conjugate;
    }
    
    public const string INSTANCE_NAME = "Master";

    protected override void HandleUnload()
    {
        check(_pendingDisposeConjugates.Count == 0);
        check(Instance == this);

        foreach (var pair in _conjugateMap)
        {
            if (pair.Value.WeakRef.TryGetTarget(out var conjugate) && conjugate.IsBlack)
            {
                conjugate.Dispose();
            }
        }
        
        Instance = null;
        
        base.HandleUnload();
    }

    private MasterAssemblyLoadContext() : base(INSTANCE_NAME)
    {
        Instance = this;

        RegisterZCall(new ZCallDispatcher_Delegate());
        RegisterZCallResolver(new ZCallResolver_Method(this), 1);
        RegisterZCallResolver(new ZCallResolver_Property(this), 2);
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
        check(registryId > 0, $"Type {managed.GetType().FullName} does not have a valid conjugate registry id");

        IntPtr unmanaged = MasterAssemblyLoadContext_Interop.BuildConjugate_Black(registryId, userdata);
        check(unmanaged != default);
        _conjugateMap[unmanaged] = new(registryId, new(managed));

        return unmanaged;
    }

    private void ReleaseConjugate_Black(IntPtr unmanaged)
    {
        verify(_conjugateMap.TryGetValue(unmanaged, out var rec));

        uint16 registryId = rec.RegistryId;
        check(registryId > 0);

        _conjugateMap.Remove(unmanaged);
        MasterAssemblyLoadContext_Interop.ReleaseConjugate_Black(registryId, unmanaged);
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
    private readonly ConcurrentQueue<IConjugate> _pendingDisposeConjugates = new();
    
    private readonly record struct ConjugateRec(uint16 RegistryId, WeakReference<IConjugate> WeakRef);

}


