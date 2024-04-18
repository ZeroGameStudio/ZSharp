namespace ZeroGames.ZSharp.Core;

public interface IConjugate : IGCHandle
{
    IntPtr Unmanaged { get; }

    static abstract IConjugate? BuildConjugate(IntPtr unmanaged);
    static bool IsValid(IConjugate? conjugate) => conjugate is not null && conjugate.bValid;

    bool bUninitialized => Unmanaged == IntPtr.Zero;
    bool bDead => Unmanaged == new IntPtr(0xDEAD);
    bool bValid => !bUninitialized && !bDead;

}


