using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp;

internal static unsafe class UE_Interop
{

    public static delegate* unmanaged<uint8, char*, void> GUE_Log;

}