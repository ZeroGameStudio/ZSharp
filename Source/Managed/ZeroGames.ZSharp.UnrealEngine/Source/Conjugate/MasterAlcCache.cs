// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealEngine;

public static class MasterAlcCache
{
	public static IMasterAssemblyLoadContext Instance { get; } = IMasterAssemblyLoadContext.Instance ?? throw new InvalidOperationException();
}


