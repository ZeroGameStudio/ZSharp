using System.Runtime.InteropServices;
using System.Runtime.Loader;

namespace ZeroGames.ZSharp.Core;

public class MasterAssemblyLoadContext : AssemblyLoadContext, IGCHandle
{

    public static MasterAssemblyLoadContext? Get()
    {
        return _GSingleton;
    }

    internal static void UnloadSingleton()
    {
        if (_GSingleton is null)
        {
            throw new Exception();
        }
        
        _GSingleton.Unload();
    }

    public MasterAssemblyLoadContext() : base("Master", true)
    {
        if (_GSingleton is not null)
        {
            throw new Exception();
        }
        
        GCHandle = GCHandle.Alloc(this, GCHandleType.Weak);
        _GSingleton = this;

        Unloading += context =>
        {
            if (_GSingleton != this)
            {
                throw new Exception();
            }
            
            _GSingleton = null;
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
    
    private static MasterAssemblyLoadContext? _GSingleton;
    
}


