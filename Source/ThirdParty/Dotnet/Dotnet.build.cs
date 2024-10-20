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
		
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "inc"));

		// Basic build info
		string platformName = Target.Platform.ToString();
		string moduleDir = ModuleDirectory;
		string zsharpDir = PluginDirectory;
		string projectDir = DirectoryReference.FromFile(Target.ProjectFile)!.FullName;
		
		// Z# constants
		// IMPORTANT: KEEP SYNC WITH ZSharpCore.build.cs
		const string BINARIES_DIR = "Binaries";
		const string CONFIG_DIR = "Config";
		const string PRE_COMPILED_DIR = "Precompiled";
		const string DOTNET_ROOT_DIR = "dotnet";
		const string HOSTFXR_DLL = "hostfxr.dll";
		const string CORECLR_DLL = "coreclr.dll";
		const string CORELIB_DLL = "System.Private.CoreLib.dll";
		const string RUNTIME_CONFIG_JSON = "ZSharp.runtimeconfig.json";
		const string NETCOREAPP = "Microsoft.NETCore.App";
		
		// Z# configurations
		// IMPORTANT: KEEP SYNC WITH ZSharpCore.build.cs
		string dotnetVersion = "8.0.3";
		string runtimeImpl = "coreclr";

		// Copy dotnet
		string libSrcDir = Path.Combine(moduleDir, "lib", platformName);
		string frameworkSrcDir = Path.Combine(moduleDir, "framework");
		string dotnetDstDir = Path.Combine(projectDir, BINARIES_DIR, platformName, DOTNET_ROOT_DIR);
		string frameworkDstDir = Path.Combine(dotnetDstDir, "shared", NETCOREAPP, dotnetVersion);
		
		{ // hostfxr
			string hostSrcDir = Path.Combine(libSrcDir, "host");
			string hostDstDir = Path.Combine(dotnetDstDir, "host", "fxr", dotnetVersion);
			
			string hostfxrSrcPath = Path.Combine(hostSrcDir, HOSTFXR_DLL);
			string hostfxrDstPath = Path.Combine(hostDstDir, HOSTFXR_DLL);
			RuntimeDependencies.Add(hostfxrDstPath, hostfxrSrcPath);
		}
		
		{ // coreclr & CoreLib
			string runtimeSrcDir = Path.Combine(libSrcDir, "runtime", runtimeImpl);
			string runtimeDstDir = frameworkDstDir;
			
			string coreLibSrcPath = Path.Combine(runtimeSrcDir, CORELIB_DLL);
			string coreLibDstPath = Path.Combine(runtimeDstDir, CORELIB_DLL);
			RuntimeDependencies.Add(coreLibDstPath, coreLibSrcPath);
			
			string runtimeSrcPath = Path.Combine(runtimeSrcDir, CORECLR_DLL);
			string runtimeDstPath = Path.Combine(runtimeDstDir, CORECLR_DLL);
			RuntimeDependencies.Add(runtimeDstPath, runtimeSrcPath);
		}
		
		{ // BCL
            IEnumerable<string> frameworkFiles = GetFiles(frameworkSrcDir);
            foreach (var file in frameworkFiles)
            {
	            string relativePath = GetRelativePath(file, frameworkSrcDir);
            	RuntimeDependencies.Add(Path.Combine(frameworkDstDir, relativePath), file);
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
	}

	private static IEnumerable<string> GetFiles(string directory)
	{
		if (!Directory.Exists(directory))
		{
			return Array.Empty<string>();
		}
		
		var files = new List<string>();

		var strings = Directory.GetFiles(directory);

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