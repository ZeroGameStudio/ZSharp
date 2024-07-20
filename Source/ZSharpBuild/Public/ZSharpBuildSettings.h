// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSharpBuildSettings.generated.h"

/**
 * 
 */
UCLASS(Config = ZSharpBuild, DefaultConfig)
class ZSHARPBUILD_API UZSharpBuildSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	virtual FName GetContainerName() const override { return TEXT("Editor"); }
	virtual FName GetCategoryName() const override { return TEXT("ZSharp"); }

public:
	const TArray<FString>& GetPluginsToScanZsproj() const { return PluginsToScanZsproj; }

private:
	UPROPERTY(Config, EditAnywhere, Category = "Solution")
	TArray<FString> PluginsToScanZsproj;
	
};


