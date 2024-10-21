// Copyright Zero Games. All Rights Reserved.


#include "ZSharpExportSettings.h"

void UZSharpExportSettings::PostInitProperties()
{
	Super::PostInitProperties();

	InvalidateCache();
}

void UZSharpExportSettings::PostReloadConfig(FProperty* PropertyThatWasLoaded)
{
	Super::PostReloadConfig(PropertyThatWasLoaded);
	
	InvalidateCache();
}

#if WITH_EDITOR

void UZSharpExportSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
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


