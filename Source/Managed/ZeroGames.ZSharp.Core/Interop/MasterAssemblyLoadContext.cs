// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

public class MasterAssemblyLoadContext : ZSharpAssemblyLoadContextBase
{

    public const string KName = "__Master";
    
    public static MasterAssemblyLoadContext? Get()
    {
        return _sSingleton;
    }
    
    public unsafe int32 ZCall(ZCallHandle handle, ZCallBuffer* buffer)
    {
        return MasterAssemblyLoadContext_Interop.SZCallByHandle(handle, buffer);
    }

    public unsafe int32 ZCall(string name, ZCallBuffer* buffer, bool bResolve = true)
    {
        fixed (char* data = name.ToCharArray())
        {
            ZCallHandle handle;
            return MasterAssemblyLoadContext_Interop.SZCallByName(data, buffer, &handle, (uint8)(bResolve ? 1 : 0));
        }
    }

    public unsafe ZCallHandle PrecacheZCall(string name)
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
    
    private static MasterAssemblyLoadContext? _sSingleton;
    
    private MasterAssemblyLoadContext() : base(KName)
    {
        _sSingleton = this;

        Unloading += context =>
        {
            if (_sSingleton != context)
            {
                throw new Exception();
            }

            _sSingleton = null;
            
            Logger.Log($"Master ALC Unloaded, name: {Name}, handle: {GCHandle.ToIntPtr(GCHandle)}");
        };
    }
    
}


