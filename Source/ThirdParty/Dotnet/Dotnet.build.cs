// Copyright Zero Games. All Rights Reserved.

using System;
using System.Collections.Generic;
using System.IO;
using EpicGames.Core;
using UnrealBuildTool;

public class Dotnet : ModuleRules
{
	public Dotnet(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;
		
		PublicSystemIncludePaths.Add(Path.Combine(ModuleDirectory, "inc"));

		// Basic build info
		string platformName = Target.Platform.ToString();
		string moduleDir = ModuleDirectory;
		string zsharpDir = PluginDirectory;
		string projectDir = DirectoryReference.FromFile(Target.ProjectFile)!.FullName;
		
		// Z# constants
		const string BINARIES_DIR = "Binaries";
		const string CONFIG_DIR = "Config";
		const string PRE_COMPILED_DIR = "Precompiled";
		const string DOTNET_ROOT_DIR = "dotnet";
		const string HOSTFXR_DLL = "hostfxr.dll";
		const string RUNTIME_CONFIG_JSON = "ZSharp.runtimeconfig.json";
		const string NETCOREAPP = "Microsoft.NETCore.App";
		
		// Z# configurations
		const bool FORCE_USE_MONO = false;
		const bool USE_MONO_FOR_EDITOR_BUILD = false;
		const string DOTNET_VERSION = "9.0.0";
		
		// Select runtime implementation
		string runtimeImpl = FORCE_USE_MONO || Target.bBuildEditor && USE_MONO_FOR_EDITOR_BUILD ? "mono" : platformName switch
		{
			"Win64" or "Linux" or "LinuxArm64" => "coreclr",
			_ => "mono",
		};

		// Copy runtime
		string dotnetRoot = Path.Combine(moduleDir, "runtime", DOTNET_VERSION);
		string nativeSrcDir = Path.Combine(dotnetRoot, "native", platformName);
		string dotnetDstDir = Path.Combine(projectDir, BINARIES_DIR, platformName, DOTNET_ROOT_DIR);
		
		{ // hostfxr
			string hostfxrSrcPath = Path.Combine(nativeSrcDir, HOSTFXR_DLL);
			string hostfxrDstPath = Path.Combine(dotnetDstDir, "host", "fxr", DOTNET_VERSION, HOSTFXR_DLL);
			RuntimeDependencies.Add(hostfxrDstPath, hostfxrSrcPath);
		}
		
		{ // native
			string runtimeSrcDir = Path.Combine(nativeSrcDir, runtimeImpl);
			string runtimeDstDir = Path.Combine(dotnetDstDir, "shared", NETCOREAPP, DOTNET_VERSION);
			
			IEnumerable<string> libs = GetFiles(runtimeSrcDir);
			foreach (var lib in libs)
			{
				string relativePath = GetRelativePath(lib, runtimeSrcDir);
				RuntimeDependencies.Add(Path.Combine(runtimeDstDir, relativePath), lib);
			}
		}
		
		{ // lib
			string libSrcDir = Path.Combine(dotnetRoot, "lib");
			string libDstDir = Path.Combine(dotnetDstDir, "shared", NETCOREAPP, DOTNET_VERSION);
			
            IEnumerable<string> libs = GetFiles(libSrcDir);
            foreach (var lib in libs)
            {
	            string relativePath = GetRelativePath(lib, libSrcDir);
            	RuntimeDependencies.Add(Path.Combine(libDstDir, relativePath), lib);
            }
		}
		
		{ // ZSharp.runtimeconfig.json
			string runtimeConfigDstPath = Path.Combine(projectDir, CONFIG_DIR, RUNTIME_CONFIG_JSON);
			string defaultRuntimeConfigPath = Path.Combine(zsharpDir, CONFIG_DIR, RUNTIME_CONFIG_JSON);
			if (File.Exists(runtimeConfigDstPath))
			{
				RuntimeDependencies.Add(runtimeConfigDstPath);
			}
			else
			{
				RuntimeDependencies.Add(runtimeConfigDstPath, defaultRuntimeConfigPath);
			}
		}
		
		// Copy precompiled assemblies
		string managedDstDir = Path.Combine(projectDir, BINARIES_DIR, "Managed");
		HashSet<string> existingLibs = new();
		{
			string precompiledDir = Path.Combine(zsharpDir, PRE_COMPILED_DIR);
			IEnumerable<string> precompileds = GetFiles(precompiledDir);
			foreach (var precompiled in precompileds)
			{
				string relativePath = GetRelativePath(precompiled, precompiledDir);
				string dstPath = Path.Combine(managedDstDir, relativePath);
				// Multiple precompiled dirs have the same assembly, copy only one.
				if (!existingLibs.Add(NormalizePath(dstPath)))
				{
					continue;
				}
				
				RuntimeDependencies.Add(dstPath, precompiled);
			}
		}
		
		// Marks all the rest managed assemblies as runtime dependency.
		{
			IEnumerable<string> assemblies = GetFiles(managedDstDir);
			foreach (var assembly in assemblies)
			{
				string relativePath = GetRelativePath(assembly, managedDstDir);
				string dstPath = Path.Combine(managedDstDir, relativePath);
				// This assembly is already marked by precompiled.
				if (existingLibs.Contains(dstPath))
				{
					continue;
				}
				
				RuntimeDependencies.Add(dstPath);
			}
		}
		
		// Predefined macros.
		if (runtimeImpl is "coreclr")
		{
			PublicDefinitions.Add("ZSHARP_WITH_CORECLR=1");
			PublicDefinitions.Add("ZSHARP_WITH_MONO=0");
		}
		else
		{
			PublicDefinitions.Add("ZSHARP_WITH_CORECLR=0");
			PublicDefinitions.Add("ZSHARP_WITH_MONO=1");
		}
		
		int interp = Target.Platform == UnrealTargetPlatform.IOS ? 1 : 0;
		PublicDefinitions.Add($"ZSHARP_WITH_INTERPRETER={interp}");
		
		PublicDefinitions.Add($"ZSHARP_DOTNET_PATH_TO_PROJECT=\"Binaries/{Target.Platform}/dotnet\"");
		PublicDefinitions.Add($"ZSHARP_HOSTFXR_PATH_TO_DOTNET=\"host/fxr/{DOTNET_VERSION}/hostfxr.dll\"");
		PublicDefinitions.Add($"ZSHARP_RUNTIME_PATH_TO_DOTNET=\"shared/Microsoft.NETCore.App/{DOTNET_VERSION}/coreclr.dll\"");
		PublicDefinitions.Add($"ZSHARP_RUNTIME_CONFIG_FILE_NAME=\"{RUNTIME_CONFIG_JSON}\"");
	}

	private static IEnumerable<string> GetFiles(string directory, string pattern = "*.*")
	{
		if (!Directory.Exists(directory))
		{
			return Array.Empty<string>();
		}
		
		var files = new List<string>();

		var strings = Directory.GetFiles(directory, pattern);

		foreach (var file in strings)
		{
			files.Add(file);
		}

		foreach (var file in Directory.GetDirectories(directory))
		{
			files.AddRange(GetFiles(file));
		}

		return files;
	}

	private static string GetRelativePath(string file, string relative)
	{
		return file.Substring(relative.Length + 1, file.Length - relative.Length - 1);
	}

	private static string NormalizePath(string path)
	{
		return path.Replace('\\', '/');
	}
	
}


