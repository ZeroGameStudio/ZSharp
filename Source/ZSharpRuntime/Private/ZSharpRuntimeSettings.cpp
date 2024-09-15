// Copyright Zero Games. All Rights Reserved.


#include "ZSharpRuntimeSettings.h"

UZSharpRuntimeSettings::UZSharpRuntimeSettings()
{
	IntrinsicModuleAssemblyMapping.Emplace("Core", { EngineAssemblyName, false });
	IntrinsicModuleAssemblyMapping.Emplace("CoreUObject", { EngineAssemblyName, false });
	
	IntrinsicModuleAssemblyMapping.Emplace("PhysicsCore", { EngineAssemblyName, false });
	IntrinsicModuleAssemblyMapping.Emplace("InputCore", { EngineAssemblyName, false });
	
	IntrinsicModuleAssemblyMapping.Emplace("Engine", { EngineAssemblyName, false });
}

const FZModuleMappingContext* UZSharpRuntimeSettings::GetModuleMappingContext(const FString& module) const
{
	const FZModuleMappingContext* ctx = IntrinsicModuleAssemblyMapping.Find(module);
	if (!ctx)
	{
		ctx = ModuleAssemblyMapping.Find(module);
	}

	return ctx;
}

void UZSharpRuntimeSettings::ForeachMappedModule(TFunctionRef<void(const FString&, const FZModuleMappingContext&)> action) const
{
	for (const auto& pair : IntrinsicModuleAssemblyMapping)
	{
		action(pair.Key, pair.Value);
	}

	for (const auto& pair : ModuleAssemblyMapping)
	{
		action(pair.Key, pair.Value);
	}
}

#if WITH_EDITORONLY_DATA

TArray<FString> UZSharpRuntimeSettings::GetModuleOptions()
{
	TArray<FModuleStatus> statuses;
	FModuleManager::Get().QueryModules(statuses);

	TArray<FString> moduleNames;
	for (const auto& status : statuses)
	{
		moduleNames.Emplace(status.Name);
	}

	moduleNames.StableSort();

	return moduleNames;
}

#endif


