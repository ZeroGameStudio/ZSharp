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

		if (_paths is null)
		{
			GConfig.TryGetArray(GZSharpIni, "Managed.AssemblyResolver", "Paths", out var paths);
			paths ??= [];
			
			string[]? editorPaths = null;
			if (UnrealBuild.WithEditor)
			{
				GConfig.TryGetArrayByFileName(GZSharpIni, "Managed.AssemblyResolver", "EditorPaths", out editorPaths);
			}
			editorPaths ??= [];
			
			_paths = 
			[ 
				$"{UnrealPaths.GetPluginDir("ZSharp")}/Managed",
				$"{UnrealPaths.ProjectDir}/Binaries/Managed",
				$"{UnrealPaths.ProjectDir}/Managed",
				..paths.Select(p => $"{UnrealPaths.ProjectDir}/{p}"),
				..editorPaths.Select(p => $"{UnrealPaths.ProjectDir}/{p}")
			];
		}

		string? dllPath = null;
		foreach (var path in _paths)
		{
			if (TryGetDllPath(path, name, out dllPath))
			{
				break;
			}
		}

		return dllPath;
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

	private string[]? _paths;

}


