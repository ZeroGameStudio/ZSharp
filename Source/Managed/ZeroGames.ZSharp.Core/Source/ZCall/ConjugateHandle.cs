// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

public readonly struct ConjugateHandle
{

    public static ConjugateHandle FromConjugate(IConjugate? conjugate) => conjugate is not null ? new(conjugate.GCHandle) : new();
    public GCHandle ToGCHandle() => GCHandle.FromIntPtr(_handle);
    public T? ToConjugate<T>() where T : class, IConjugate => ToGCHandle().Target as T;

    public bool Valid => _handle != IntPtr.Zero;

    private ConjugateHandle(GCHandle handle)
    {
        _handle = GCHandle.ToIntPtr(handle);
    }

    private readonly IntPtr _handle;
    
}


