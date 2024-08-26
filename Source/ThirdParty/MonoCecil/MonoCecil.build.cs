// Copyright Zero Games. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class MonoCecil : ModuleRules
{
	public MonoCecil(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;
		
		string src = Path.Combine(ModuleDirectory, "Mono.Cecil.dll");
		string dst = "$(BinaryOutputDir)/../Managed/Shared/Mono.Cecil.dll";
		RuntimeDependencies.Add(dst, src);
	}
}