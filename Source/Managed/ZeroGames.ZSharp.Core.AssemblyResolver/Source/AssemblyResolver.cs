// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;
using System.IO;
using System.Runtime.Loader;

[assembly: AssemblyResolver(typeof(ZeroGames.ZSharp.Core.AssemblyResolver.AssemblyResolver))]

namespace ZeroGames.ZSharp.Core.AssemblyResolver;

internal class AssemblyResolver : IAssemblyResolver
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
		
		_cachedManagedDllDir ??= $"{UnrealPaths.ProjectDir}/Binaries/Managed";
		
		TryGetDllPath(_cachedManagedDllDir, name, out var path);
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
			if (baseDir == _cachedManagedDllDir && _sharedDirs.Contains(new DirectoryInfo(dir).Name))
			{
				continue;
			}
			
			if (TryGetDllPath(dir, assemblyName, out result))
			{
				return true;
			}
		}

		return false;
	}

	private static readonly HashSet<string> _sharedDirs = [ "Core", "ForwardShared", "DeferredShared" ];

	private string? _cachedManagedDllDir;

}


