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

		string PlatformName = Target.Platform.ToString();
		string DotnetVersion = "8.0.3";

		string DotnetRoot = Path.Combine("$(BinaryOutputDir)", "dotnet");
		string HostDir = Path.Combine(DotnetRoot, "host", "fxr", DotnetVersion);
		string FrameworkDir = Path.Combine(DotnetRoot, "shared", "Microsoft.NETCore.App", DotnetVersion);

		string HostfxrSrcPath = Path.Combine(ModuleDirectory, "lib", PlatformName, "host", "hostfxr.dll");
		string HostfxrDstPath = Path.Combine(HostDir, "hostfxr.dll");
		RuntimeDependencies.Add(HostfxrDstPath, HostfxrSrcPath);
		
		string RuntimeName = "coreclr";
		
		string RuntimeSrcPath = Path.Combine(ModuleDirectory, "lib", PlatformName, "runtime", RuntimeName, "coreclr.dll");
		string RuntimeDstPath = Path.Combine(FrameworkDir, "coreclr.dll");
		RuntimeDependencies.Add(RuntimeDstPath, RuntimeSrcPath);
		
		string CoreLibSrcPath = Path.Combine(ModuleDirectory, "lib", PlatformName, "runtime", RuntimeName, "System.Private.CoreLib.dll");
		string CoreLibDstPath = Path.Combine(FrameworkDir, "System.Private.CoreLib.dll");
		RuntimeDependencies.Add(CoreLibDstPath, CoreLibSrcPath);

		string FrameworkSrcDir = Path.Combine(ModuleDirectory, "framework");
		IEnumerable<string> FrameworkFiles = GetFiles(FrameworkSrcDir);
		foreach (var File in FrameworkFiles)
		{
			string RelativePath = File.Substring(FrameworkSrcDir.Length + 1, File.Length - FrameworkSrcDir.Length - 1);

			RuntimeDependencies.Add(Path.Combine(FrameworkDir, RelativePath), File);
		}
	}

	private static IEnumerable<string> GetFiles(string InDirectory, string InPattern = "*.*")
	{
		var Files = new List<string>();

		var Strings = Directory.GetFiles(InDirectory, InPattern);

		foreach (var File in Strings)
		{
			Files.Add(File);
		}

		foreach (var File in Directory.GetDirectories(InDirectory))
		{
			Files.AddRange(GetFiles(File, InPattern));
		}

		return Files;
	}
}