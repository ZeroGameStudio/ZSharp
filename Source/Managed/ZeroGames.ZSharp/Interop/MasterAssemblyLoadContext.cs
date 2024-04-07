using System.Runtime.InteropServices;
using System.Runtime.Loader;

namespace ZeroGames.ZSharp;

public class MasterAssemblyLoadContext : AssemblyLoadContext, IConjugate
{

    public static MasterAssemblyLoadContext? Get()
    {
        return _GSingleton;
    }

    public MasterAssemblyLoadContext() : base("Master", true)
    {
        if (_GSingleton is not null)
        {
            throw new Exception();
        }
        
        GCHandle = GCHandle.Alloc(this);
        _GSingleton = this;
    }

    public void Release()
    {
        if (_GSingleton != this)
        {
            throw new Exception();
        }
        
        GCHandle.Free();
        _GSingleton = null;
    }

    public GCHandle GCHandle { get; }
    
    private static MasterAssemblyLoadContext? _GSingleton;
    
}


