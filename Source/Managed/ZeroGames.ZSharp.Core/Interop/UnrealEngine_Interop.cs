// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core;

internal static class UnrealEngine_Interop
{

    public static unsafe delegate* unmanaged<uint8, char*, void> SLog = null;
    public static unsafe delegate* unmanaged<uint8> SIsInGameThread = null;

}


