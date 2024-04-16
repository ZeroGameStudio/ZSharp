using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

public readonly struct ConjugateHandle
{

    public static ConjugateHandle FromGCHandle(GCHandle handle) => new(handle);
    public GCHandle ToGCHandle() => GCHandle.FromIntPtr(_handle);

    private ConjugateHandle(GCHandle handle)
    {
        _handle = GCHandle.ToIntPtr(handle);
    }

    private readonly IntPtr _handle;
    
}


