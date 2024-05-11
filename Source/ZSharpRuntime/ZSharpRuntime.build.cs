// Copyright Zero Games. All Rights Reserved.

using UnrealBuildTool;

public class ZSharpRuntime : ModuleRules
{
	public ZSharpRuntime(ReadOnlyTargetRules Target) : base(Target)
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
				"Engine",
			}
		);
		
		PrivateDependencyModuleNames.AddRange
		(
			new string[]
			{
				"CoreUObject",
				"DeveloperSettings",
				
				"ZSharpCore",
			}
		);
		
		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"UnrealEd",
					
					"ZSharpExport",
				}
			);
		}
		
		DynamicallyLoadedModuleNames.AddRange
		(
			new string[]
			{
			}
		);
		
		PublicDefinitions.Add($"ZSHARP_ENGINE_ASSEMBLY_NAME=\"ZeroGames.ZSharp.UnrealEngine\"");
	}
}
