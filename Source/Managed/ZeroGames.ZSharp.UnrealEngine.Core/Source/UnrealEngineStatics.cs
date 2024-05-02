// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine.Core;

public static class UnrealEngineStatics
{

    public static unsafe bool IsInGameThread() => UnrealEngine_Interop.SIsInGameThread() > 0;

}


