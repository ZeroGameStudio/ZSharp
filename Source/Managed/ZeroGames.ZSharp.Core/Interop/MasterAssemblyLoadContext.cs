// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

public class MasterAssemblyLoadContext : ZSharpAssemblyLoadContextBase
{

    public const string KName = "Master";
    
    public static MasterAssemblyLoadContext? Get()
    {
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

    protected override void HandleUnload()
    {
        base.HandleUnload();
        
        if (_sSingleton != this)
        {
            throw new Exception();
        }

        _sSingleton = null;
            
        Logger.Log($"Master ALC Unloaded, name: {Name}, handle: {GCHandle.ToIntPtr(GCHandle)}");
    }

    private static MasterAssemblyLoadContext? _sSingleton;
    
    private MasterAssemblyLoadContext() : base(KName)
    {
        _sSingleton = this;
    }
    
}


