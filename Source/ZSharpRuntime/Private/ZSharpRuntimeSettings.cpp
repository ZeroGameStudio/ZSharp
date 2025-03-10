// Copyright Zero Games. All Rights Reserved.

#include "ZSharpRuntimeSettings.h"

void UZSharpRuntimeSettings::ForEachEmitVirtualModule(TFunctionRef<void(const FZEmitVirtualModule&)> action) const
{
	for (const auto& module : EmitVirtualModules)
	{
		action(module);
	}
}

int32 UZSharpRuntimeSettings::GetModuleEmitMetadataSources(const FString& moduleName, TArray<FZModuleEmitMetadataSource>& result) const
{
	result.Reset();
	ModuleEmitMetadataSourceLookup.MultiFind(FName { moduleName }, result, true);
	
	return result.Num();
}

#if WITH_EDITOR

TArray<FString> UZSharpRuntimeSettings::GetModuleOptions()
{
	TArray<FModuleStatus> statuses;
	FModuleManager::Get().QueryModules(statuses);

	TArray<FString> moduleNames;
	for (const auto& status : statuses)
	{
		moduleNames.Emplace(status.Name);
	}

	for (const auto& module : GetDefault<ThisClass>()->EmitVirtualModules)
	{
		moduleNames.Emplace(module.ModuleName);
	}

	moduleNames.StableSort();

	return moduleNames;
}

#endif

void UZSharpRuntimeSettings::PostInitProperties()
{
	Super::PostInitProperties();

	InvalidateCache();
}

void UZSharpRuntimeSettings::PostReloadConfig(FProperty* reloadedProperty)
{
	Super::PostReloadConfig(reloadedProperty);

	InvalidateCache();
}

#if WITH_EDITOR

void UZSharpRuntimeSettings::PostEditChangeProperty(FPropertyChangedEvent& event)
{
	Super::PostEditChangeProperty(event);

	InvalidateCache();
}

#endif

void UZSharpRuntimeSettings::InvalidateCache()
{
	ModuleEmitMetadataSourceLookup.Reset();
	for (const auto& source : ModuleEmitMetadataSources)
	{
		ModuleEmitMetadataSourceLookup.Emplace(FName { source.ModuleName }, source);
	}
}


