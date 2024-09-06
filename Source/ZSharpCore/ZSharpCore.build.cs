// Copyright Zero Games. All Rights Reserved.

using UnrealBuildTool;

public class ZSharpCore : ModuleRules
{
	public ZSharpCore(ReadOnlyTargetRules Target) : base(Target)
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
		
		if (Target.Platform == UnrealTargetPlatform.Win64
		    || Target.Platform == UnrealTargetPlatform.Linux
		    || Target.Platform == UnrealTargetPlatform.Mac)
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
		
		PublicDefinitions.Add($"ZSHARP_PLATFORM=\"{Target.Platform}\"");
		
		PublicDefinitions.Add($"ZSHARP_DOTNET_VERSION=\"{dotnetVersion}\"");
		PublicDefinitions.Add($"ZSHARP_DOTNET_PATH=\"{Target.Platform}/dotnet\"");
		PublicDefinitions.Add($"ZSHARP_HOSTFXR_PATH=\"host/fxr/{dotnetVersion}/hostfxr.dll\"");
		PublicDefinitions.Add($"ZSHARP_RUNTIME_CONFIG_FILE_NAME=\"{runtimeConfigFileName}\"");
		
		PublicDefinitions.Add($"ZSHARP_CORE_ASSEMBLY_NAME=\"ZeroGames.ZSharp.Core\"");
		PublicDefinitions.Add($"ZSHARP_CORE_ENGINE_ASSEMBLY_NAME=\"ZeroGames.ZSharp.Core.UnrealEngine\"");
		PublicDefinitions.Add($"ZSHARP_RESOLVER_ASSEMBLY_NAME=\"ZeroGames.ZSharp.AssemblyResolver\"");
	}
}
