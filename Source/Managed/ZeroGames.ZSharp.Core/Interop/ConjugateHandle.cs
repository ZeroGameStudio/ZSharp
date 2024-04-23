// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

public readonly struct ConjugateHandle
{

    public static ConjugateHandle FromConjugate(IConjugate? conjugate) => conjugate is not null ? new(conjugate.GCHandle) : new();
    public static ConjugateHandle FromGCHandle(GCHandle handle) => new(handle);
    public GCHandle ToGCHandle() => GCHandle.FromIntPtr(_handle);

    public bool bValid => _handle != IntPtr.Zero;

    private ConjugateHandle(GCHandle handle)
    {
        _handle = GCHandle.ToIntPtr(handle);
    }

    private readonly IntPtr _handle;
    
}


