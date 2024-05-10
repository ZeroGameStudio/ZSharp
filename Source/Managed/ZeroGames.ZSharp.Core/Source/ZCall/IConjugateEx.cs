// Copyright Zero Games. All Rights Reserved.

using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp.Core;

public static class IConjugateEx
{

    public static ConjugateHandle GetConjugateHandle(this IConjugate? @this) => ConjugateHandle.FromConjugate(@this);
    public static bool IsUninitialized(this IConjugate @this) => @this.Unmanaged == IntPtr.Zero;
    public static bool IsDead(this IConjugate @this) => @this.GCHandle == new GCHandle();
    public static bool IsValid(this IConjugate? @this) => @this is not null && !@this.IsUninitialized() && !@this.IsDead();

}


