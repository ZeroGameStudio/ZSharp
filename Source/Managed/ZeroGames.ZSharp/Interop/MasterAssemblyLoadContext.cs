using System.Runtime.InteropServices;
using System.Runtime.Loader;

namespace ZeroGames.ZSharp;

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

    public GCHandle GCHandle { get; }
    
    private static MasterAssemblyLoadContext? _GSingleton;
    
}


