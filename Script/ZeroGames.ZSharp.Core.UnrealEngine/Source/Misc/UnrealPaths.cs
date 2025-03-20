// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.UnrealEngine;

public static class UnrealPaths
{

	public static unsafe string GetPluginDir(string pluginName)
	{
		using InteropString dir = new();
		fixed (char* pluginNamePtr = pluginName)
		{
			Path_Interop.GetPluginDir(pluginNamePtr, dir.Address);
		}

		return dir;
	}

	public static unsafe string ProjectDir
	{
		get
		{
			using InteropString dir = new();
			Path_Interop.GetProjectDir(dir.Address);
			return dir;
		}
	}
	
}


