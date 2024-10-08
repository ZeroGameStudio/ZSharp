// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;
using System.IO;
using System.Runtime.Loader;

[assembly: AssemblyResolver(typeof(ZeroGames.ZSharp.AssemblyResolver.AssemblyResolver))]

namespace ZeroGames.ZSharp.AssemblyResolver;

public class AssemblyResolver : IAssemblyResolver
{
	
	public string? Resolve(string name)
	{
		foreach (var assembly in AssemblyLoadContext.Default.Assemblies)
		{
			if (assembly.GetName().Name == name && !string.IsNullOrWhiteSpace(assembly.Location))
			{
				return assembly.Location;
			}
		}
		
		_cachedManagedDllDir ??= $"{UnrealPaths.ProjectDir}Binaries/Managed";
		if (!TryGetDllPath(_cachedManagedDllDir, name, out var path))
		{
			// Scan shared and precompiled dir for editor build.
			if (UnrealBuild.WithEditor)
			{
				string pluginDir = UnrealPaths.GetPluginDir("ZSharp");
				if (!TryGetDllPath($"{pluginDir}/Binaries/Managed/Shared", name, out path))
				{
					TryGetDllPath($"{pluginDir}/Precompiled", name, out path);
				}
			}
		}

		return path;
	}

	private bool TryGetDllPath(string baseDir, string assemblyName, [NotNullWhen(true)] out string? result)
	{
		result = null;

		if (!Directory.Exists(baseDir))
		{
			return false;
		}
		
		foreach (var path in Directory.GetFiles(baseDir))
		{
			if (Path.GetFileNameWithoutExtension(path) == assemblyName)
			{
				result = path;
				return true;
			}
		}

		foreach (var dir in Directory.GetDirectories(baseDir))
		{
			if (TryGetDllPath(dir, assemblyName, out result))
			{
				return true;
			}
		}

		return false;
	}

	private string? _cachedManagedDllDir;

}


