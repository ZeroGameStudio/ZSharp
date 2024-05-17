// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "ZSharpExportSettings.generated.h"

/**
 * 
 */
UCLASS(Config = ZSharpEditor, DefaultConfig)
class ZSHARPEXPORT_API UZSharpExportSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UZSharpExportSettings();

public:
	virtual FName GetContainerName() const override { return TEXT("Editor"); }
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
