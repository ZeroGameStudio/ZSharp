using System.Reflection;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

internal class InteropProxy<T> : IGCHandle where T : class
{
    public InteropProxy(T inner)
    {
        GCHandle = GCHandle.Alloc(this, GCHandleType.Weak);
        Inner = inner;
    }

    public GCHandle GCHandle { get; private set; }
    public T Inner { get; }
    
}