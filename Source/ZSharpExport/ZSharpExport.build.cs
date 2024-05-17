// Copyright Zero Games. All Rights Reserved.

using UnrealBuildTool;

public class ZSharpExport : ModuleRules
{
	public ZSharpExport(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
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
				
				"ZSharpCore",
			}
		);
		
		DynamicallyLoadedModuleNames.AddRange
		(
			new string[]
			{
			}
		);
		
		PublicDefinitions.Add($"ZSHARP_ENGINE_ASSEMBLY_NAME=\"ZeroGames.ZSharp.UnrealEngine\"");
	}
}
