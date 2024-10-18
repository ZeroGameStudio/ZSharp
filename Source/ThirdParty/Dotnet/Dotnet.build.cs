// Copyright Zero Games. All Rights Reserved.

using System;
using System.Collections.Generic;
using System.IO;
using UnrealBuildTool;

public class Dotnet : ModuleRules
{
	public Dotnet(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "inc"));

		string platformName = Target.Platform.ToString();
		string dotnetVersion = "8.0.3";

		string dotnetRoot = Path.Combine(PluginDirectory, "Binaries", platformName, "dotnet");

		{ // Copy hostfxr.
			string hostDir = Path.Combine(dotnetRoot, "host", "fxr", dotnetVersion);
			
			string hostfxrSrcPath = Path.Combine(ModuleDirectory, "lib", platformName, "host", "hostfxr.dll");
			string hostfxrDstPath = Path.Combine(hostDir, "hostfxr.dll");
			RuntimeDependencies.Add(hostfxrDstPath, hostfxrSrcPath);
		}
		
		string frameworkDir = Path.Combine(dotnetRoot, "shared", "Microsoft.NETCore.App", dotnetVersion);
		
		{ // Copy coreclr & CoreLib
			string runtimeName = "coreclr";
			
			string coreLibSrcPath = Path.Combine(ModuleDirectory, "lib", platformName, "runtime", runtimeName, "System.Private.CoreLib.dll");
			string coreLibDstPath = Path.Combine(frameworkDir, "System.Private.CoreLib.dll");
			RuntimeDependencies.Add(coreLibDstPath, coreLibSrcPath);
			
			string runtimeSrcPath = Path.Combine(ModuleDirectory, "lib", platformName, "runtime", runtimeName, "coreclr.dll");
			string runtimeDstPath = Path.Combine(frameworkDir, "coreclr.dll");
			RuntimeDependencies.Add(runtimeDstPath, runtimeSrcPath);
		}

		{ // Copy BCL
			string frameworkSrcDir = Path.Combine(ModuleDirectory, "framework");
            IEnumerable<string> frameworkFiles = GetFiles(frameworkSrcDir);
            foreach (var file in frameworkFiles)
            {
            	string relativePath = file.Substring(frameworkSrcDir.Length + 1, file.Length - frameworkSrcDir.Length - 1);
            
            	RuntimeDependencies.Add(Path.Combine(frameworkDir, relativePath), file);
            }
		}

		HashSet<string> existingLibs = new();
		{ // Copy shared libs
			string sharedDstRoot = Target.bBuildEditor ? $"{PluginDirectory}/Binaries" : "$(TargetOutputDir)/..";
			string sharedSrcDir = Path.Combine(ModuleDirectory, "shared");
			string sharedDstDir = Path.Combine(sharedDstRoot, "Managed", "Shared");
			IEnumerable<string> sharedFiles = GetFiles(sharedSrcDir);
			foreach (var file in sharedFiles)
			{
				string relativePath = file.Substring(sharedSrcDir.Length + 1, file.Length - sharedSrcDir.Length - 1);
				string dstPath = Path.Combine(sharedDstDir, relativePath);
				RuntimeDependencies.Add(dstPath, file);
				existingLibs.Add(NormalizePath(dstPath));
			}
		}

		/*
		 * On staged builds:
		 * 1. Copy all managed libs to project binaries dir.
		 * 2. Mark runtimeconfig.json as runtime dependency.
		 */
		if (!Target.bBuildEditor)
		{
			string managedDir = Path.Combine(PluginDirectory, "Binaries/Managed");
			IEnumerable<string> managedAssemblies = GetFiles(managedDir);
			foreach (var file in managedAssemblies)
			{
				string relativePath = file.Substring(managedDir.Length + 1, file.Length - managedDir.Length - 1);
				string dstPath = Path.Combine("$(TargetOutputDir)/../Managed", relativePath);
				if (existingLibs.Contains(NormalizePath(dstPath)))
				{
					continue;
				}
				
				RuntimeDependencies.Add(dstPath, file);
			}
			
			RuntimeDependencies.Add(Path.Combine(PluginDirectory, "Config/ZSharp.runtimeconfig.json"));
		}
	}

	private static IEnumerable<string> GetFiles(string directory, string pattern = "*.*")
	{
		var files = new List<string>();

		var strings = Directory.GetFiles(directory, pattern);

		foreach (var file in strings)
		{
			files.Add(file);
		}

		foreach (var file in Directory.GetDirectories(directory))
		{
			files.AddRange(GetFiles(file, pattern));
		}

		return files;
	}

	private static string NormalizePath(string path)
	{
		return path.Replace('\\', '/');
	}
	
}