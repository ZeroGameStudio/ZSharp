using System.Runtime.InteropServices;
using System.Runtime.Loader;

namespace ZeroGames.ZSharp.Core;

public class MasterAssemblyLoadContext : AssemblyLoadContext, IGCHandle
{

    public static MasterAssemblyLoadContext? Get()
    {
        return _sSingleton;
    }

    internal static void UnloadSingleton()
    {
        if (_sSingleton is null)
        {
            throw new Exception();
        }
        
        _sSingleton.Unload();
    }

    public MasterAssemblyLoadContext() : base("Master", true)
    {
        if (_sSingleton is not null)
        {
            throw new Exception();
        }
        
        GCHandle = GCHandle.Alloc(this, GCHandleType.Weak);
        _sSingleton = this;

        Unloading += context =>
        {
            if (_sSingleton != this)
            {
                throw new Exception();
            }
            
            _sSingleton = null;
        };
    }

    public unsafe int32 ZCall(ZCallHandle handle, ZCallBuffer* buffer)
    {
        return MasterAssemblyLoadContext_Interop.GZCallByHandle(handle, buffer);
    }

    public unsafe int32 ZCall(string name, ZCallBuffer* buffer, bool bResolve = true)
    {
        fixed (char* data = name.ToCharArray())
        {
            ZCallHandle handle;
            return MasterAssemblyLoadContext_Interop.GZCallByName(data, buffer, &handle, (uint8)(bResolve ? 1 : 0));
        }
    }

    public unsafe ZCallHandle PrecacheZCall(string name)
    {
        fixed (char* data = name.ToCharArray())
        {
            return MasterAssemblyLoadContext_Interop.GGetZCallHandle(data);
        }
    }

    public GCHandle GCHandle { get; }
    
    private static MasterAssemblyLoadContext? _sSingleton;
    
}


