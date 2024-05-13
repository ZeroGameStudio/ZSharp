// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;
using System.Reflection;
using System.Runtime.Loader;
using System.Security;

namespace ZeroGames.ZSharp.Core;

internal class MasterAssemblyLoadContext : ZSharpAssemblyLoadContextBase, IMasterAssemblyLoadContext
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

    public unsafe int32 ZCall(ZCallHandle handle, ZCallBuffer* buffer) => ZCall_Black(handle, buffer);
    public ZCallHandle GetZCallHandle(string name) => GetZCallHandle_Black(name);
    public IntPtr BuildConjugate(uint16 registryId, IConjugate managed) => BuildConjugate_Black(registryId, managed);
    public void ReleaseConjugate(uint16 registryId, IntPtr unmanaged) => ReleaseConjugate_Black(registryId, unmanaged);

    internal static MasterAssemblyLoadContext Create()
    {
        if (_sSingleton is not null)
        {
            throw new InvalidOperationException("Master ALC already exists.");
        }

        return new();
    }
    
    internal unsafe int32 ZCall_Red(ZCallHandle handle, ZCallBuffer* buffer)
    {
        throw new NotImplementedException();
    }

    internal ZCallHandle GetZCallHandle_Red(string name)
    {
        throw new NotImplementedException();
    }

    internal IntPtr BuildConjugate_Red(IntPtr unmanaged, Type type)
    {
        Type conjugateType = typeof(IConjugate<>).MakeGenericType(type);
        if (type.IsAssignableTo(conjugateType))
        {
            type.GetMethod("BuildConjugate")!.Invoke(null, new object[] { unmanaged });
            return unmanaged;
        }

        return new();
    }

    internal void ReleaseConjugate_Red(IntPtr unmanaged)
    {
        if (!_conjugateMap.Remove(unmanaged, out var wref))
        {
            throw new InvalidOperationException($"Conjugate [{unmanaged}] does not exists.");
        }

        if (!wref.TryGetTarget(out var conjugate))
        {
            throw new InvalidOperationException($"Conjugate [{unmanaged}] has already dead.");
        }
        
        conjugate.Release();
    }

    internal IConjugate? Conjugate(IntPtr unmanaged)
    {
        if (!_conjugateMap.Remove(unmanaged, out var wref))
        {
            throw new InvalidOperationException($"Conjugate [{unmanaged}] does not exists.");
        }

        wref.TryGetTarget(out var conjugate);
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
    
    private unsafe int32 ZCall_Black(ZCallHandle handle, ZCallBuffer* buffer)
    {
        return MasterAssemblyLoadContext_Interop.SZCall_Black(handle, buffer);
    }

    private unsafe ZCallHandle GetZCallHandle_Black(string name)
    {
        fixed (char* data = name.ToCharArray())
        {
            return MasterAssemblyLoadContext_Interop.SGetZCallHandle_Black(data);
        }
    }

    private unsafe IntPtr BuildConjugate_Black(uint16 registryId, IConjugate managed)
    {
        IntPtr unmanaged = MasterAssemblyLoadContext_Interop.SBuildConjugate_Black(registryId);
        if (unmanaged != IntPtr.Zero)
        {
            _conjugateMap[unmanaged] = new(managed);
        }

        return unmanaged;
    }

    private unsafe void ReleaseConjugate_Black(uint16 registryId, IntPtr unmanaged)
    {
        _conjugateMap.Remove(unmanaged);
        MasterAssemblyLoadContext_Interop.SReleaseConjugate_Black(registryId, unmanaged);
    }

    private const int32 KDefaultConjugateMapCapacity = 65536;

    private static MasterAssemblyLoadContext? _sSingleton;
    
    private MasterAssemblyLoadContext() : base(KName)
    {
        _sSingleton = this;
    }

    private Dictionary<IntPtr, WeakReference<IConjugate>> _conjugateMap = new(KDefaultConjugateMapCapacity);

}


