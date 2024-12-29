// Copyright Zero Games. All Rights Reserved.


#include "ZSharpRuntimeSettings.h"

int32 UZSharpRuntimeSettings::GetModuleEmitMetadataSource(const FString& moduleName, TArray<FZModuleEmitMetadataSource>& result) const
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


