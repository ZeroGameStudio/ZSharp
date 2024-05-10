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
        string err = "Code has no permission to access Master ALC.";
        StackFrame stack = new(1);
        MethodBase? method = stack.GetMethod();
        if (method is null)
        {
            throw new SecurityException(err);
        }

        Type? type = method.DeclaringType;
        if (type is null)
        {
            throw new SecurityException(err);
        }

        AssemblyLoadContext? callerAlc = GetLoadContext(type.Assembly);
        if (callerAlc != _sSingleton && callerAlc != Default)
        {
            throw new SecurityException(err);
        }
        
        return _sSingleton;
    }
    
    public unsafe int32 ZCall(ZCallHandle handle, ZCallBuffer* buffer)
    {
        return MasterAssemblyLoadContext_Interop.SZCallByHandle(handle, buffer);
    }

    public unsafe int32 ZCall(string name, ZCallBuffer* buffer)
    {
        fixed (char* data = name.ToCharArray())
        {
            return MasterAssemblyLoadContext_Interop.SZCallByName(data, buffer, null);
        }
    }
    
    public unsafe int32 ZCall(string name, ZCallBuffer* buffer, out ZCallHandle handle)
    {
        fixed (char* data = name.ToCharArray())
        {
            ZCallHandle fixedHandle;
            int32 res = MasterAssemblyLoadContext_Interop.SZCallByName(data, buffer, &fixedHandle);
            handle = fixedHandle;
            return res;
        }
    }

    public unsafe void ZCall_AnyThread(ZCallHandle handle, ZCallBuffer* buffer, int32 numSlots)
    {
        MasterAssemblyLoadContext_Interop.SZCallByHandle_AnyThread(handle, buffer, numSlots);
    }

    public unsafe ZCallHandle GetZCallHandle(string name)
    {
        fixed (char* data = name.ToCharArray())
        {
            return MasterAssemblyLoadContext_Interop.SGetZCallHandle(data);
        }
    }

    internal static MasterAssemblyLoadContext Create()
    {
        if (_sSingleton is not null)
        {
            throw new Exception();
        }

        return new();
    }

    protected override void HandleUnload()
    {
        base.HandleUnload();
        
        if (_sSingleton != this)
        {
            throw new Exception();
        }

        _sSingleton = null;
    }

    private static MasterAssemblyLoadContext? _sSingleton;
    
    private MasterAssemblyLoadContext() : base(KName)
    {
        _sSingleton = this;
    }
    
}


