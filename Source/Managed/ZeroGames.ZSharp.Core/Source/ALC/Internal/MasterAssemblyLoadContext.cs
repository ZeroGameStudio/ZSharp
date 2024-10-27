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
                    throw new InvalidOperationException();
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
        ZCallHandle handle = ZCallHandle.Alloc();
        _zcallMap[handle] = dispatcher;
        _zcallName2Handle[dispatcher.Name] = handle;

        return handle;
    }

    public void RegisterZCallResolver(IZCallResolver resolver, uint64 priority)
    {
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

    public EZCallErrorCode ZCall(ZCallHandle handle, ZCallBuffer* buffer) => ZCall_Black(handle, buffer);
    public ZCallHandle GetZCallHandle(string name) => GetZCallHandle_Black(name);
    public IntPtr BuildConjugate(IConjugate managed, IntPtr userdata) => BuildConjugate_Black(managed, userdata);
    public void ReleaseConjugate(IConjugate conjugate) => ReleaseConjugate_Black(conjugate.Unmanaged);
    
    public void PushPendingDisposeConjugate(IConjugate conjugate)
    {
        _pendingDisposeConjugates.Enqueue(conjugate);
    }

    public void Tick(float deltaTime)
    {
        while (_pendingDisposeConjugates.TryDequeue(out var conjugate))
        {
            conjugate.Dispose();
        }
    }
    
    public EZCallErrorCode ZCall_Red(ZCallHandle handle, ZCallBuffer* buffer)
    {
        if (!_zcallMap.TryGetValue(handle, out var dispatcher))
        {
            return EZCallErrorCode.DispatcherNotFound;
        }

        return dispatcher.Dispatch(buffer);
    }

    public ZCallHandle GetZCallHandle_Red(string name)
    {
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
        if (!_conjugateMap.Remove(unmanaged, out var rec))
        {
            throw new InvalidOperationException($"Conjugate [{unmanaged}] does not exists.");
        }

        if (!rec.WeakRef.TryGetTarget(out var conjugate))
        {
            throw new InvalidOperationException($"Conjugate [{unmanaged}] has already dead.");
        }
        
        conjugate.Release();
    }

    public IConjugate? Conjugate(IntPtr unmanaged)
    {
        if (!_conjugateMap.TryGetValue(unmanaged, out var rec))
        {
            throw new InvalidOperationException($"Conjugate [{unmanaged}] does not exists.");
        }

        rec.WeakRef.TryGetTarget(out var conjugate);
        return conjugate;
    }
    
    public const string INSTANCE_NAME = "Master";

    protected override void HandleUnload()
    {
        if (Instance != this)
        {
            throw new InvalidOperationException("Master ALC mismatch.");
        }

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
        if (registryId == 0)
        {
            throw new InvalidOperationException($"ConjugateRegistryId of type {managed.GetType().FullName} is 0.");
        }

        IntPtr unmanaged = MasterAssemblyLoadContext_Interop.BuildConjugate_Black(registryId, userdata);
        if (unmanaged != IntPtr.Zero)
        {
            _conjugateMap[unmanaged] = new(registryId, new(managed));
        }
        else
        {
            throw new InvalidOperationException();
        }

        return unmanaged;
    }

    private void ReleaseConjugate_Black(IntPtr unmanaged)
    {
        if (!_conjugateMap.TryGetValue(unmanaged, out var rec))
        {
            throw new InvalidOperationException($"Conjugate [{unmanaged}] does not exists.");
        }

        uint16 registryId = rec.RegistryId;
        if (registryId == 0)
        {
            throw new InvalidOperationException($"ConjugateRegistryId of unmanaged {unmanaged} is 0.");
        }

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


