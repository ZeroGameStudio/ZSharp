// Copyright Zero Games. All Rights Reserved.

using UnrealBuildTool;

public class ZSharpCore : ModuleRules
{
	public ZSharpCore(ReadOnlyTargetRules Target) : base(Target)
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
				
				"Dotnet",
			}
		);
		
		DynamicallyLoadedModuleNames.AddRange
		(
			new string[]
			{
			}
		);

		PublicDefinitions.Add("ZSHARP_CORE_ASSEMBLY_NAME=\"ZeroGames.ZSharp.Core\"");
		PublicDefinitions.Add("ZSHARP_RESOLVER_ASSEMBLY_NAME=\"ZeroGames.ZSharp.Core.AssemblyResolver\"");
		PublicDefinitions.Add("ZSHARP_CORE_ENGINE_ASSEMBLY_NAME=\"ZeroGames.ZSharp.Core.UnrealEngine\"");
		PublicDefinitions.Add("ZSHARP_CORE_ASYNC_ASSEMBLY_NAME=\"ZeroGames.ZSharp.Core.Async\"");
	}
}


