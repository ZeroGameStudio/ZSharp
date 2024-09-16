// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

public interface IConjugate : IGCHandle, IExplicitLifecycle, IDisposable
{
    public const IntPtr DEAD_ADDR = 0xDEAD;
    protected internal void Release();
    IntPtr Unmanaged { get; }
    bool IsBlack { get; }
}

public interface IConjugate<out T> : IConjugate where T : class, IConjugate<T>
{
    static abstract T BuildConjugate(IntPtr unmanaged);
}


