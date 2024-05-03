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
				"Projects",
				
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

		string dotnetVersion = "8.0.3";
		string runtimeConfigFileName = "ZSharp.runtimeconfig.json";
		
		PrivateDefinitions.Add($"ZSHARP_PLATFORM=\"{Target.Platform}\"");
		
		PrivateDefinitions.Add($"ZSHARP_DOTNET_VERSION=\"{dotnetVersion}\"");
		PrivateDefinitions.Add($"ZSHARP_DOTNET_PATH=\"{Target.Platform}/dotnet\"");
		PrivateDefinitions.Add($"ZSHARP_HOSTFXR_PATH=\"host/fxr/{dotnetVersion}/hostfxr.dll\"");
		PrivateDefinitions.Add($"ZSHARP_RUNTIME_CONFIG_FILE_NAME=\"{runtimeConfigFileName}\"");
		
		PrivateDefinitions.Add($"ZSHARP_CORE_ASSEMBLY_NAME=\"ZeroGames.ZSharp.Core\"");
		PrivateDefinitions.Add($"ZSHARP_ENGINE_CORE_ASSEMBLY_NAME=\"ZeroGames.ZSharp.UnrealEngine.Core\"");
	}
}
