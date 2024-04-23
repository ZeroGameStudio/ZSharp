// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

public static class InteropString_Interop
{

    public static unsafe delegate* unmanaged<char*, IntPtr> GAlloc;
    public static unsafe delegate* unmanaged<IntPtr, void> GFree;
    public static unsafe delegate* unmanaged<IntPtr, char*> GGetData;
    public static unsafe delegate* unmanaged<IntPtr, char*, void> GSetData;

}


