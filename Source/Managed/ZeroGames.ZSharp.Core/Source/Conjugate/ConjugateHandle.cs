// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

[StructLayout(LayoutKind.Sequential)]
public readonly struct ConjugateHandle : IEquatable<ConjugateHandle>
{

    public static ConjugateHandle FromConjugate(IConjugate? conjugate)
    {
        if (conjugate is { IsExpired: true })
        {
            throw new InvalidOperationException("Access expired conjugate.");
        }
        
        return new(conjugate);
    }
    
    public override bool Equals(object? obj)
    {
        return obj is ConjugateHandle other && Equals(other);
    }

    public override int32 GetHashCode()
    {
        return _handle.GetHashCode();
    }
    
    public bool Equals(ConjugateHandle other)
    {
        return _handle == other._handle;
    }

    public T? GetTarget<T>() where T : class, IConjugate
    {
        if (!IsValid)
        {
            return null;
        }
        
        MasterAssemblyLoadContext? alc = MasterAssemblyLoadContext.Instance;
        if (alc is null)
        {
            return null;
        }

        return alc.Conjugate(_handle) as T;
    }

    public T GetTargetChecked<T>() where T : class, IConjugate
    {
        if (!IsValid)
        {
            throw new InvalidOperationException();
        }
        
        MasterAssemblyLoadContext? alc = MasterAssemblyLoadContext.Instance;
        if (alc is null)
        {
            throw new InvalidOperationException();
        }
        
        var result = alc.Conjugate(_handle) as T;
        if (result is null)
        {
            throw new InvalidOperationException();
        }
        
        return result;
    }
    
    public bool IsValid => _handle != IntPtr.Zero;
    
    private ConjugateHandle(IConjugate? conjugate)
    {
        if (conjugate is not null)
        {
            _handle = conjugate.Unmanaged;
        }
    }

    private readonly IntPtr _handle;
    
}


