using System.Runtime.InteropServices;

namespace ZeroGames.ZSharp;

internal static class UE_Interop
{

    public static unsafe delegate* unmanaged<uint8, char*, void> GUE_Log;

}