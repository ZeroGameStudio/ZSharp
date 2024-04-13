namespace ZeroGames.ZSharp;

public static unsafe class InteropString_Interop
{

    public static delegate* unmanaged<char*, IntPtr> GAlloc;
    public static delegate* unmanaged<IntPtr, void> GFree;
    public static delegate* unmanaged<IntPtr, char*> GGetData;
    public static delegate* unmanaged<IntPtr, char*, void> GSetData;

}