// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "ZSharpRuntimeSettings.generated.h"

USTRUCT()
struct FZModuleMappingContext
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString AssemblyName;

	UPROPERTY(EditAnywhere)
	bool bHasDynamicFields;
	
};

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
	
public:
	const FZModuleMappingContext* GetModuleMappingContext(const FString& module) const;
	void ForeachMappedModule(TFunctionRef<void(const FString&, const FZModuleMappingContext&)> action) const;

private:
	UPROPERTY(Transient, VisibleAnywhere, Category = "Assembly")
	FString CoreAssemblyName = ZSHARP_CORE_ASSEMBLY_NAME;

	UPROPERTY(Transient, VisibleAnywhere, Category = "Assembly")
	FString EngineCoreAssemblyName = ZSHARP_CORE_ENGINE_ASSEMBLY_NAME;
	
	UPROPERTY(Transient, VisibleAnywhere, Category = "Assembly")
	FString EngineAssemblyName = ZSHARP_ENGINE_ASSEMBLY_NAME;
	
	UPROPERTY(Config, EditAnywhere, Category = "Mapping")
	TMap<FString, FZModuleMappingContext> ModuleAssemblyMapping;

	UPROPERTY(Transient, VisibleAnywhere, Category = "Mapping")
	TMap<FString, FZModuleMappingContext> IntrinsicModuleAssemblyMapping;
	
};


