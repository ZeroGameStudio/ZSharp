// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

public readonly struct ConjugateHandle
{

    public ConjugateHandle(IConjugate? conjugate)
    {
        if (conjugate is not null)
        {
            _handle = conjugate.Unmanaged;
        }
    }

    public override int32 GetHashCode() => _handle.GetHashCode();

    public T? GetTarget<T>() where T : class, IConjugate
    {
        MasterAssemblyLoadContext? alc = MasterAssemblyLoadContext.Get();
        if (alc is null)
        {
            return null;
        }

        return alc.Conjugate(_handle) as T;
    }
    
    public bool Valid => _handle != IntPtr.Zero;

    private readonly IntPtr _handle;
    
}


