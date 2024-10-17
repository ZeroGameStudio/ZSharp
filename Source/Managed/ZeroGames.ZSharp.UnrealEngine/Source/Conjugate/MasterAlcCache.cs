// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.Loader;

namespace ZeroGames.ZSharp.UnrealEngine;

public static class MasterAlcCache
{
	public static IMasterAssemblyLoadContext Instance { get; }

	static MasterAlcCache()
	{
		Assembly asm = Assembly.GetExecutingAssembly();
		AssemblyLoadContext? alc = AssemblyLoadContext.GetLoadContext(asm);
		if (alc is null)
		{
			throw new Exception("Owning ALC not found.");
		}

		if (alc is not IMasterAssemblyLoadContext masterAlc)
		{
			throw new Exception($"Owning ALC is not MasterAssemblyLoadContext but {alc.GetType().Name}");
		}

		if (alc != IMasterAssemblyLoadContext.Instance)
		{
			throw new Exception("Owning ALC is MasterAssemblyLoadContext but not the live one.");
		}
		
		Instance = masterAlc;
	}
}


