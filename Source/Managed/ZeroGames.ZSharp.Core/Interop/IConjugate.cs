// Copyright Zero Games. All Rights Reserved.

using System.Xml;

namespace ZeroGames.ZSharp.Core;

public interface IConjugate : IGCHandle
{
    const int32 KDeadAddress = 0xDEAD;
    
    IntPtr Unmanaged { get; }

    static bool IsValid(IConjugate? conjugate) => conjugate is not null && conjugate.bValid;

    bool bUninitialized => Unmanaged == IntPtr.Zero;
    bool bDead => Unmanaged == KDeadAddress;
    bool bValid => !bUninitialized && !bDead;

}

public interface IConjugate<out T> : IConjugate where T : class, IConjugate<T>
{
    static abstract T BuildConjugate(IntPtr unmanaged);
}


