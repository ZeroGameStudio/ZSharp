// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

internal static class InteropString_Interop
{

    public static unsafe delegate* unmanaged<char*, IntPtr> Alloc = null;
    public static unsafe delegate* unmanaged<IntPtr, void> Free = null;
    public static unsafe delegate* unmanaged<IntPtr, char*> GetData = null;
    public static unsafe delegate* unmanaged<IntPtr, char*, void> SetData = null;

}


