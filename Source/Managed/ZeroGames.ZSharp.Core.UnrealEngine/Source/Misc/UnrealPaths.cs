// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.UnrealEngine;

public static class UnrealPaths
{

	public static unsafe string ProjectDir
	{
		get
		{
			using InteropString dir = new();
			Path_Interop.GetProjectDir(dir.Address);
			return dir.Data;
		}
	}
	
}


