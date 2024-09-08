// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "ZSharpEmitSettings.generated.h"

USTRUCT()
struct FZAssembliesToScan
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FString> Assemblies;
};

/**
 * 
 */
UCLASS(Config = ZSharp, DefaultConfig)
class ZSHARPEMIT_API UZSharpEmitSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	virtual FName GetCategoryName() const override { return TEXT("ZSharp"); }

public:
	TArray<FString> GetModuleAssembliesToScan(const FName& moduleName) const;
	
private:
	UPROPERTY(Config, EditAnywhere, Category = "Emit")
	TMap<FName, FZAssembliesToScan> ModuleAssembliesToScanMap;
	
};


