// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

public interface IConjugate : IDisposable, IExplicitLifecycle
{
	protected internal void Release();
	GCHandle GCHandle { get; }
    IntPtr Unmanaged { get; }
    bool IsBlack { get; }
}

public interface IConjugate<out T> : IConjugate where T : class, IConjugate<T>
{
    static abstract T BuildConjugate(IntPtr unmanaged);
}


