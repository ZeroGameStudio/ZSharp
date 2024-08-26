// Copyright Zero Games. All Rights Reserved.

using System.IO;

[assembly: AssemblyResolver(typeof(ZeroGames.ZSharp.AssemblyResolver.AssemblyResolver))]

namespace ZeroGames.ZSharp.AssemblyResolver;

public class AssemblyResolver : IAssemblyResolver
{
	
	public string? Resolve(string name)
	{
		_cachedManagedDllDir ??= $"{UnrealPaths.ProjectDir}Binaries/Managed";

		foreach (var file in Directory.GetFiles(_cachedManagedDllDir, "*.dll"))
		{
			if (Path.GetFileNameWithoutExtension(file) == name)
			{
				return file;
			}
		}

		return null;
	}

	private string? _cachedManagedDllDir;
	
}


