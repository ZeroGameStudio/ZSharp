// Copyright Zero Games. All Rights Reserved.

using System.Collections.Concurrent;
using System.Diagnostics;
using System.Reflection;
using System.Runtime.Loader;
using System.Security;

namespace ZeroGames.ZSharp.Core;

internal unsafe class MasterAssemblyLoadContext : ZSharpAssemblyLoadContextBase, IMasterAssemblyLoadContext
{

    public const string KName = "Master";
    
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

    public Type? GetType(string assemblyName, string typeName)
    {
        Assembly? asm = Assemblies.FirstOrDefault(asm => asm.GetName().Name == assemblyName);
        if (asm is null)
        {
            return null;
        }

        return asm.GetType(typeName);
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

    public int32 ZCall(ZCallHandle handle, ZCallBuffer* buffer) => ZCall_Black(handle, buffer);
    public ZCallHandle GetZCallHandle(string name) => GetZCallHandle_Black(name);
    public IntPtr BuildConjugate(IConjugate managed) => BuildConjugate_Black(managed);
    public void ReleaseConjugate(IntPtr unmanaged) => ReleaseConjugate_Black(unmanaged);
    
    public void PushPendingDisposeConjugate(IConjugate conjugate)
    {
        _pendingDisposeConjugates.Enqueue(conjugate);
    }

    internal static MasterAssemblyLoadContext Create()
    {
        if (_sSingleton is not null)
        {
            throw new InvalidOperationException("Master ALC already exists.");
        }

        return new();
    }

    internal void Tick(float deltaTime)
    {
        while (_pendingDisposeConjugates.TryDequeue(out var conjugate))
        {
            conjugate.Dispose();
        }
    }
    
    internal int32 ZCall_Red(ZCallHandle handle, ZCallBuffer* buffer)
    {
        if (!_zcallMap.TryGetValue(handle, out var dispatcher))
        {
            return -1;
        }

        return dispatcher.Dispatch(buffer);
    }

    internal ZCallHandle GetZCallHandle_Red(string name)
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

    internal IntPtr BuildConjugate_Red(IntPtr unmanaged, Type type)
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

    internal void ReleaseConjugate_Red(IntPtr unmanaged)
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

    internal IConjugate? Conjugate(IntPtr unmanaged)
    {
        if (!_conjugateMap.TryGetValue(unmanaged, out var rec))
        {
            throw new InvalidOperationException($"Conjugate [{unmanaged}] does not exists.");
        }

        rec.Wref.TryGetTarget(out var conjugate);
        return conjugate;
    }

    protected override void HandleUnload()
    {
        base.HandleUnload();
        
        if (_sSingleton != this)
        {
            throw new InvalidOperationException("Master ALC mismatch.");
        }

        _sSingleton = null;
    }

    private const int32 _kDefaultConjugateMapCapacity = 65536;

    private static MasterAssemblyLoadContext? _sSingleton;
    
    private MasterAssemblyLoadContext() : base(KName)
    {
        _sSingleton = this;

        Resolving += (_, fullName) => LoadAssembly(fullName.Name!);

        RegisterZCall(new ZCallDispatcher_Delegate());
        RegisterZCallResolver(new ZCallResolver_Method(), 1);
        RegisterZCallResolver(new ZCallResolver_Property(), 2);
    }
    
    private int32 ZCall_Black(ZCallHandle handle, ZCallBuffer* buffer)
    {
        return MasterAssemblyLoadContext_Interop.SZCall_Black(handle, buffer);
    }

    private ZCallHandle GetZCallHandle_Black(string name)
    {
        fixed (char* data = name.ToCharArray())
        {
            return MasterAssemblyLoadContext_Interop.SGetZCallHandle_Black(data);
        }
    }

    private IntPtr BuildConjugate_Black(IConjugate managed)
    {
        uint16 registryId = GetTypeConjugateRegistryId(managed.GetType());
        if (registryId == 0)
        {
            throw new InvalidOperationException($"ConjugateRegistryId of type {managed.GetType().FullName} is 0.");
        }
        
        IntPtr unmanaged = MasterAssemblyLoadContext_Interop.SBuildConjugate_Black(registryId);
        if (unmanaged != IntPtr.Zero)
        {
            _conjugateMap[unmanaged] = new(registryId, new(managed));
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
    
    private Dictionary<ZCallHandle, IZCallDispatcher> _zcallMap = new();
    private Dictionary<string, ZCallHandle> _zcallName2Handle = new();
    private List<(IZCallResolver Resolver, uint64 Priority)> _zcallResolverLink = new();
    private Dictionary<IntPtr, ConjugateRec> _conjugateMap = new(_kDefaultConjugateMapCapacity);
    private Dictionary<Type, uint16> _conjugateRegistryIdLookup = new();
    private ConcurrentQueue<IConjugate> _pendingDisposeConjugates = new();
    
    private readonly record struct ConjugateRec(uint16 RegistryId, WeakReference<IConjugate> Wref);

}


