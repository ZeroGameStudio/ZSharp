﻿// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;
using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

public interface IConjugate : IDisposable
{
	protected internal void Release();
	GCHandle GCHandle { get; }
    IntPtr Unmanaged { get; }
    bool IsBlack { get; }
    bool IsExpired { get; }
}

public interface IConjugate<out T> : IConjugate where T : class, IConjugate<T>
{
	static abstract T BuildConjugate(IntPtr unmanaged);
}

public static class ConjugateExtensions
{
	public static bool IsValid([NotNullWhen(true)] this IConjugate? conjugate) => conjugate is { IsExpired: false };
}


