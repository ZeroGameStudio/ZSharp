// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

public interface IConjugate : IGCHandle
{
    protected internal void ReleaseConjugate();
    IntPtr Unmanaged { get; }
}

public interface IConjugate<out T> : IConjugate where T : class, IConjugate<T>
{
    static abstract T BuildConjugate(IntPtr unmanaged);
}


