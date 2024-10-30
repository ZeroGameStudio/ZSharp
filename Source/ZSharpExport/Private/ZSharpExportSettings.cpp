// Copyright Zero Games. All Rights Reserved.


#include "ZSharpExportSettings.h"

void UZSharpExportSettings::PostInitProperties()
{
	Super::PostInitProperties();

	InvalidateCache();
}

void UZSharpExportSettings::PostReloadConfig(FProperty* reloadedProperty)
{
	Super::PostReloadConfig(reloadedProperty);
	
	InvalidateCache();
}

#if WITH_EDITOR

void UZSharpExportSettings::PostEditChangeProperty(FPropertyChangedEvent& event)
{
	Super::PostEditChangeProperty(event);
	
	InvalidateCache();
}

#endif

void UZSharpExportSettings::InvalidateCache()
{
	ForceExportFieldPathsHash.Reset();
	for (const auto& path : ForceExportFieldPaths)
	{
		ForceExportFieldPathsHash.Add(path);
	}
	
	ForceNotExportFieldPathsHash.Reset();
	for (const auto& path : ForceNotExportFieldPaths)
	{
		ForceNotExportFieldPathsHash.Add(path);
	}
}


