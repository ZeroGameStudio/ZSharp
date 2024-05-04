// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ZSharpRuntimeSettings.generated.h"

/**
 * 
 */
UCLASS(Config = ZSharp, DefaultConfig)
class ZSHARPRUNTIME_API UZSharpRuntimeSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UZSharpRuntimeSettings();

public:
	virtual FName GetCategoryName() const override { return TEXT("ZSharp"); }

private:
	UPROPERTY(Transient, VisibleAnywhere, Category = "Assembly")
	FString CoreAssemblyName = ZSHARP_CORE_ASSEMBLY_NAME;

	UPROPERTY(Transient, VisibleAnywhere, Category = "Assembly")
	FString EngineCoreAssemblyName = ZSHARP_ENGINE_CORE_ASSEMBLY_NAME;
	
	UPROPERTY(Transient, VisibleAnywhere, Category = "Assembly")
	FString EngineAssemblyName = ZSHARP_ENGINE_ASSEMBLY_NAME;
	
	UPROPERTY(Config, EditAnywhere, Category = "Export")
	TMap<FString, FString> ModuleAssemblyMapping;

	UPROPERTY(Transient, VisibleAnywhere, Category = "Export")
	TMap<FString, FString> IntrinsicModuleAssemblyMapping;
	
};


