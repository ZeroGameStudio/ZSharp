// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using System.Runtime.Loader;

namespace ZeroGames.ZSharp.UnrealEngine;

public static class MasterAlcCache
{
	public static void GuardInvariant()
	{
		Thrower.ThrowIfNotInGameThread();
		Instance.GuardUnloaded();
	}
	
	public static IMasterAssemblyLoadContext Instance { get; }

	static MasterAlcCache()
	{
		Assembly asm = Assembly.GetExecutingAssembly();
		AssemblyLoadContext? alc = AssemblyLoadContext.GetLoadContext(asm);

		Instance = (IMasterAssemblyLoadContext)alc!;
	}
}


