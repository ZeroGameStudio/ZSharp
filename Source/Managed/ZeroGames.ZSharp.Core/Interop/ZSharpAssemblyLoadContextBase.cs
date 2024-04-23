using System.Runtime.InteropServices;
using System.Runtime.Loader;

namespace ZeroGames.ZSharp.Core;

public abstract class ZSharpAssemblyLoadContextBase : AssemblyLoadContext, IGCHandle
{
    
    protected ZSharpAssemblyLoadContextBase(string name) : base(name, true)
    {
        GCHandle = GCHandle.Alloc(this, GCHandleType.Weak);
    }
    
    public GCHandle GCHandle { get; }
    
}