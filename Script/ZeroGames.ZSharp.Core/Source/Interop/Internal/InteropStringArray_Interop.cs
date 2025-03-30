// Copyright Zero Games. All Rights Reserved.

#pragma warning disable CS0649 // Field is never assigned to, and will always have its default value

namespace ZeroGames.ZSharp.Core;

internal static class InteropStringArray_Interop
{

    public static unsafe delegate* unmanaged[SuppressGCTransition]<IntPtr> Alloc;
    public static unsafe delegate* unmanaged[SuppressGCTransition]<IntPtr, void> Free;
    public static unsafe delegate* unmanaged[SuppressGCTransition]<IntPtr, int32> Count;
    public static unsafe delegate* unmanaged[SuppressGCTransition]<IntPtr, int32, char*> Get;
    public static unsafe delegate* unmanaged[SuppressGCTransition]<IntPtr, int32, char*, void> Set;
    public static unsafe delegate* unmanaged[SuppressGCTransition]<IntPtr, char*, void> Add;
    public static unsafe delegate* unmanaged[SuppressGCTransition]<IntPtr, int32, char*, void> InsertAt;
    public static unsafe delegate* unmanaged[SuppressGCTransition]<IntPtr, int32, void> RemoveAt;
    public static unsafe delegate* unmanaged[SuppressGCTransition]<IntPtr, void> Clear;

}


