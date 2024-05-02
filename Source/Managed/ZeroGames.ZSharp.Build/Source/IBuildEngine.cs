// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Build;

public interface IBuildEngine
{
	Task<string> RunAsync();
	string? GetArgument(string key);
}