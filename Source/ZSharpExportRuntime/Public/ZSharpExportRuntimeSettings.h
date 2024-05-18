// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "ZSharpExportRuntimeSettings.generated.h"

/**
 * 
 */
UCLASS(Config = ZSharp, DefaultConfig)
class ZSHARPEXPORTRUNTIME_API UZSharpExportRuntimeSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UZSharpExportRuntimeSettings();

public:
	virtual FName GetCategoryName() const override { return TEXT("ZSharp"); }

public:
	bool IsModuleMapped(const FString& module) const;
	bool TryGetModuleAssembly(const FString& module, FString& outAssembly) const;
	void ForeachMappedModule(TFunctionRef<void(const FString&, const FString&)> action) const;

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


