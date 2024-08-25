// Copyright Zero Games. All Rights Reserved.

using System.IO;
using System.Reflection;
using System.Runtime.Loader;

[assembly: AssemblyResolver(typeof(ZeroGames.ZSharp.AssemblyResolver.AssemblyResolver))]

namespace ZeroGames.ZSharp.AssemblyResolver;

public class AssemblyResolver : IAssemblyResolver
{
	
	public ELoadAssemblyErrorCode Resolve(AssemblyLoadContext alc, string name, out Assembly? assembly)
	{
		_cachedManagedDllDir ??= $"{UnrealPaths.ProjectDir}Binaries/Managed";

		foreach (var file in Directory.GetFiles(_cachedManagedDllDir, "*.dll"))
		{
			if (Path.GetFileNameWithoutExtension(file) == name)
			{
				using FileStream fs = File.OpenRead(file);
				assembly = alc.LoadFromStream(fs);
				return ELoadAssemblyErrorCode.Succeed;
			}
		}

		assembly = null;
		return ELoadAssemblyErrorCode.FileNotFound;
	}

	private string? _cachedManagedDllDir;

}


