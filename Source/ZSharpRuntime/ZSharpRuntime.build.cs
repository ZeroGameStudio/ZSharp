// Copyright Zero Games. All Rights Reserved.

using UnrealBuildTool;

public class ZSharpRuntime : ModuleRules
{
	public ZSharpRuntime(ReadOnlyTargetRules Target) : base(Target)
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
				"FieldNotification",
				"Engine",
				"GameplayTags",
				"EnhancedInput",
				
				"DeveloperSettings",
				
				"ZSharpCore",
			}
		);
		
		PrivateDependencyModuleNames.AddRange
		(
			new string[]
			{
				"CoreUObject",
				"NetCore",
				
				"Json",
				"JsonUtilities",
			}
		);
		
		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"UnrealEd",
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
		PublicDefinitions.Add($"ZSHARP_SCANNER_ASSEMBLY_NAME=\"ZeroGames.ZSharp.UnrealFieldScanner\"");
	}
}


