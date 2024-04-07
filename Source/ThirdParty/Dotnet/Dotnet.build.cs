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

		string dotnetRoot = Path.Combine("$(BinaryOutputDir)", "dotnet");
		string hostDir = Path.Combine(dotnetRoot, "host", "fxr", dotnetVersion);
		string frameworkDir = Path.Combine(dotnetRoot, "shared", "Microsoft.NETCore.App", dotnetVersion);

		string hostfxrSrcPath = Path.Combine(ModuleDirectory, "lib", platformName, "host", "hostfxr.dll");
		string hostfxrDstPath = Path.Combine(hostDir, "hostfxr.dll");
		RuntimeDependencies.Add(hostfxrDstPath, hostfxrSrcPath);
		
		string runtimeName = "coreclr";
		
		string runtimeSrcPath = Path.Combine(ModuleDirectory, "lib", platformName, "runtime", runtimeName, "coreclr.dll");
		string runtimeDstPath = Path.Combine(frameworkDir, "coreclr.dll");
		RuntimeDependencies.Add(runtimeDstPath, runtimeSrcPath);
		
		string coreLibSrcPath = Path.Combine(ModuleDirectory, "lib", platformName, "runtime", runtimeName, "System.Private.CoreLib.dll");
		string coreLibDstPath = Path.Combine(frameworkDir, "System.Private.CoreLib.dll");
		RuntimeDependencies.Add(coreLibDstPath, coreLibSrcPath);

		string frameworkSrcDir = Path.Combine(ModuleDirectory, "framework");
		IEnumerable<string> frameworkFiles = GetFiles(frameworkSrcDir);
		foreach (var file in frameworkFiles)
		{
			string relativePath = file.Substring(frameworkSrcDir.Length + 1, file.Length - frameworkSrcDir.Length - 1);

			RuntimeDependencies.Add(Path.Combine(frameworkDir, relativePath), file);
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
}