// Copyright Zero Games. All Rights Reserved.

using System.Collections.Concurrent;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Runtime.Loader;
using System.Security;
using System.Threading;

namespace ZeroGames.ZSharp.Core;

internal unsafe class MasterAssemblyLoadContext : ZSharpAssemblyLoadContextBase, IMasterAssemblyLoadContext
{

    public const string KName = "Master";
    
    public static MasterAssemblyLoadContext Create()
    {
        if (_sSingleton is not null)
        {
            throw new InvalidOperationException("Master ALC already exists.");
        }

        return new();
    }
    
    public static MasterAssemblyLoadContext? Get()
    {
        Action @throw = () => throw new SecurityException("Code has no permission to access Master ALC.");
        
        StackFrame stack = new(1);
        MethodBase? method = stack.GetMethod();
        if (method is null)
        {
            @throw();
        }

        Type? type = method!.DeclaringType;
        if (type is null)
        {
            @throw();
        }

        AssemblyLoadContext? callerAlc = GetLoadContext(type!.Assembly);
        if (callerAlc != _sSingleton && callerAlc != Default)
        {
            @throw();
        }
        
        return _sSingleton;
    }

    public Assembly? LoadAssembly(string name)
    {
        throw new NotImplementedException();
    }

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
    public void ReleaseConjugate(IntPtr unmanaged) => ReleaseConjugate_Black(unmanaged);
    
    public void PushPendingDisposeConjugate(IConjugate conjugate)
    {
        _pendingDisposeConjugates.Enqueue(conjugate);
    }

    public ELoadAssemblyErrorCode LoadAssembly(Stream stream, void* args)
    {
        Assembly asm = LoadFromStream(stream);
        if (DllMainStatics.TryInvokeDllMain(asm, args, out var res) && res is not null)
        {
            return ELoadAssemblyErrorCode.Succeed;
        }

        return ELoadAssemblyErrorCode.Succeed;
    }

    public void Tick(float deltaTime)
    {
        while (_pendingDisposeConjugates.TryDequeue(out var conjugate))
        {
            conjugate.Dispose();
        }
        
        _curSyncContext.Tick(deltaTime);
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

        if (!rec.Wref.TryGetTarget(out var conjugate))
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

        rec.Wref.TryGetTarget(out var conjugate);
        return conjugate;
    }
    
    public SynchronizationContext SynchronizationContext
    {
        get => _curSyncContext;
    }

    protected override void HandleUnload()
    {
        if (_sSingleton != this)
        {
            throw new InvalidOperationException("Master ALC mismatch.");
        }

        if (SynchronizationContext.Current != SynchronizationContext)
        {
            throw new InvalidOperationException("SynchronizationContext mismatch.");
        }
        
        System.Threading.SynchronizationContext.SetSynchronizationContext(_prevSyncContext);

        _sSingleton = null;
        
        base.HandleUnload();
    }

    private const int32 _kDefaultConjugateMapCapacity = 65536;

    private static MasterAssemblyLoadContext? _sSingleton;
    
    private MasterAssemblyLoadContext() : base(KName)
    {
        _sSingleton = this;

        Resolving += (_, fullName) => LoadAssembly(fullName.Name!);
        
        _prevSyncContext = SynchronizationContext.Current;
        _curSyncContext = new();
        SynchronizationContext.SetSynchronizationContext(SynchronizationContext);

        RegisterZCall(new ZCallDispatcher_Delegate());
        RegisterZCallResolver(new ZCallResolver_Method(), 1);
        RegisterZCallResolver(new ZCallResolver_Property(), 2);
    }
    
    private EZCallErrorCode ZCall_Black(ZCallHandle handle, ZCallBuffer* buffer)
    {
        return (EZCallErrorCode)MasterAssemblyLoadContext_Interop.SZCall_Black(handle, buffer);
    }

    private ZCallHandle GetZCallHandle_Black(string name)
    {
        fixed (char* data = name.ToCharArray())
        {
            return MasterAssemblyLoadContext_Interop.SGetZCallHandle_Black(data);
        }
    }

    private IntPtr BuildConjugate_Black(IConjugate managed, IntPtr userdata)
    {
        uint16 registryId = GetTypeConjugateRegistryId(managed.GetType());
        if (registryId == 0)
        {
            throw new InvalidOperationException($"ConjugateRegistryId of type {managed.GetType().FullName} is 0.");
        }

        IntPtr unmanaged = MasterAssemblyLoadContext_Interop.SBuildConjugate_Black(registryId, userdata);
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
        MasterAssemblyLoadContext_Interop.SReleaseConjugate_Black(registryId, unmanaged);
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

    private SynchronizationContext? _prevSyncContext;
    private ZSharpSynchronizationContext _curSyncContext;
    
    private Dictionary<ZCallHandle, IZCallDispatcher> _zcallMap = new();
    private Dictionary<string, ZCallHandle> _zcallName2Handle = new();
    private List<(IZCallResolver Resolver, uint64 Priority)> _zcallResolverLink = new();
    private Dictionary<IntPtr, ConjugateRec> _conjugateMap = new(_kDefaultConjugateMapCapacity);
    private Dictionary<Type, uint16> _conjugateRegistryIdLookup = new();
    private ConcurrentQueue<IConjugate> _pendingDisposeConjugates = new();
    
    private readonly record struct ConjugateRec(uint16 RegistryId, WeakReference<IConjugate> Wref);

}


