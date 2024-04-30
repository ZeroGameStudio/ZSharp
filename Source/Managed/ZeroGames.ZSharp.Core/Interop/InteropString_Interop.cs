// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

internal static class InteropString_Interop
{

    public static unsafe delegate* unmanaged<char*, IntPtr> SAlloc = null;
    public static unsafe delegate* unmanaged<IntPtr, void> SFree = null;
    public static unsafe delegate* unmanaged<IntPtr, char*> SGetData = null;
    public static unsafe delegate* unmanaged<IntPtr, char*, void> SSetData = null;

}


