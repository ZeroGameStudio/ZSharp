// Copyright Zero Games. All Rights Reserved.

using UnrealBuildTool;

public class ZSharpBuild : ModuleRules
{
	public ZSharpBuild(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange
		(
			new string[]
			{
			}
		);
		
		PrivateIncludePaths.AddRange
		(
			new string[]
			{
			}
		);
		
		PublicDependencyModuleNames.AddRange
		(
			new string[]
			{
				"Core",
				
				"DeveloperSettings",
			}
		);
		
		PrivateDependencyModuleNames.AddRange
		(
			new string[]
			{
				"CoreUObject",
				
				"Projects",
				"Json",
				"JsonUtilities",
				
				"ZSharpCore",
				"ZSharpExport",
				"ZSharpRuntime",
			}
		);
		
		DynamicallyLoadedModuleNames.AddRange
		(
			new string[]
			{
			}
		);
		
		PublicDefinitions.Add($"ZSHARP_BUILD_ASSEMBLY_NAME=\"ZeroGames.ZSharp.Build\"");
	}
}


