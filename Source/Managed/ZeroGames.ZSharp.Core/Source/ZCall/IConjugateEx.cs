// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

public static class IConjugateEx
{

    public static bool IsUninitialized(this IConjugate @this) => @this.Unmanaged == IntPtr.Zero;
    public static bool IsDead(this IConjugate @this) => @this.Unmanaged == IConjugate.KDead;
    public static bool IsValid(this IConjugate? @this) => @this is not null && !@this.IsUninitialized() && !@this.IsDead();

}


