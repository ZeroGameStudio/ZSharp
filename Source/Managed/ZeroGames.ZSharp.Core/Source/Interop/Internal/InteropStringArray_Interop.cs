// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

internal static class InteropStringArray_Interop
{

    public static unsafe delegate* unmanaged<IntPtr> Alloc;
    public static unsafe delegate* unmanaged<IntPtr, void> Free;
    public static unsafe delegate* unmanaged<IntPtr, int32> Count;
    public static unsafe delegate* unmanaged<IntPtr, int32, char*> Get;
    public static unsafe delegate* unmanaged<IntPtr, int32, char*, void> Set;
    public static unsafe delegate* unmanaged<IntPtr, char*, void> Add;
    public static unsafe delegate* unmanaged<IntPtr, int32, char*, void> InsertAt;
    public static unsafe delegate* unmanaged<IntPtr, int32, void> RemoveAt;
    public static unsafe delegate* unmanaged<IntPtr, void> Clear;

}


