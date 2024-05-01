// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Build;

public interface IBuildTarget
{
    Task<string> BuildAsync();
}


