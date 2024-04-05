// Copyright Zero Games. All Rights Reserved.

using UnrealBuildTool;

public class ZSharpCLR : ModuleRules
{
	public ZSharpCLR(ReadOnlyTargetRules Target) : base(Target)
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
			}
		);
		
		PrivateDependencyModuleNames.AddRange
		(
			new string[]
			{
				"CoreUObject",
				"Engine",
				
				"Dotnet",
			}
		);
		
		DynamicallyLoadedModuleNames.AddRange
		(
			new string[]
			{
			}
		);
		
		if (!Target.bBuildEditor 
		    && (Target.Platform == UnrealTargetPlatform.Win64
		    || Target.Platform == UnrealTargetPlatform.Linux
		    || Target.Platform == UnrealTargetPlatform.Mac) || true)
		{
			PrivateDefinitions.Add("ZSHARP_WITH_CORECLR=1");
			PrivateDefinitions.Add("ZSHARP_WITH_MONO=0");
		}
		else
		{
			PrivateDefinitions.Add("ZSHARP_WITH_CORECLR=0");
			PrivateDefinitions.Add("ZSHARP_WITH_MONO=1");
		}
	}
}
