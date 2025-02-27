// Copyright Zero Games. All Rights Reserved.

using UnrealBuildTool;

public class ZSharpEditor : ModuleRules
{
	public ZSharpEditor(ReadOnlyTargetRules Target) : base(Target)
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
			}
		);
		
		PrivateDependencyModuleNames.AddRange
		(
			new string[]
			{
				"CoreUObject",
				"Engine",
				
				"Projects",
				"MainFrame",
				
				"ZSharpCore",
				"ZSharpRuntime",
				
				"ZSharpExport",
				"ZSharpBuild",
			}
		);
		
		DynamicallyLoadedModuleNames.AddRange
		(
			new string[]
			{
			}
		);
	}
}


