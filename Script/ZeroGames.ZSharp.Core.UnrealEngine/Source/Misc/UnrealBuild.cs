// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.UnrealEngine;

public static class UnrealBuild
{

	public static unsafe bool WithEditor => Build_Interop.WithEditor() > 0;

}


