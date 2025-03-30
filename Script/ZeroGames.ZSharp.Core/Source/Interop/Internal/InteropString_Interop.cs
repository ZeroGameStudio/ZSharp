// Copyright Zero Games. All Rights Reserved.

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

namespace ZeroGames.ZSharp.Core;

internal static class InteropString_Interop
{

    public static unsafe delegate* unmanaged[SuppressGCTransition]<char*, IntPtr> Alloc;
    public static unsafe delegate* unmanaged[SuppressGCTransition]<IntPtr, void> Free;
    public static unsafe delegate* unmanaged[SuppressGCTransition]<IntPtr, char*> GetData;
    public static unsafe delegate* unmanaged[SuppressGCTransition]<IntPtr, char*, void> SetData;

}


