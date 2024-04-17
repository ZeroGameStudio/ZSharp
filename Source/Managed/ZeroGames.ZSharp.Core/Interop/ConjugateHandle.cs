using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

public readonly struct ConjugateHandle
{

    public static ref readonly ConjugateHandle Invalid => ref _sInvalid;

    public static ConjugateHandle FromConjugate(IConjugate conjugate) => new(conjugate.GCHandle);
    public static ConjugateHandle FromGCHandle(GCHandle handle) => new(handle);
    public GCHandle ToGCHandle() => GCHandle.FromIntPtr(_handle);

    public bool bValid => _handle != IntPtr.Zero;

    private static ConjugateHandle _sInvalid = new(new());

    private ConjugateHandle(GCHandle handle)
    {
        _handle = GCHandle.ToIntPtr(handle);
    }

    private readonly IntPtr _handle;
    
}


