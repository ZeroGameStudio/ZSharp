// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "ZSharpRuntimeSettings.generated.h"

USTRUCT()
struct FZModuleMappingContext
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (GetOptions = "ZSharpRuntime.ZSharpRuntimeSettings.GetModuleOptions"))
	FString ModuleName;

	UPROPERTY(EditAnywhere)
	FString AssemblyName;

	UPROPERTY(EditAnywhere, meta = (ConfigRestartRequired = true))
	bool bHasDynamicFields = false;
	
};

USTRUCT()
struct FZFieldNameRedirector
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString SourceName;

	UPROPERTY(EditAnywhere)
	FString TargetName;
	
};

USTRUCT()
struct FZMasterAlcStartupAssembly
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString AssemblyName;

	UPROPERTY(EditAnywhere)
	TArray<FString> Arguments;
	
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

	FString GetFieldAlias(const FString& path) const;

	void ForeachMasterAlcStartupAssembly(TFunctionRef<void(const FZMasterAlcStartupAssembly&)> action) const;

#if WITH_EDITOR
private:
	UFUNCTION()
	static TArray<FString> GetModuleOptions();
#endif

private:
	virtual void PostInitProperties() override;
	virtual void PostReloadConfig(FProperty* PropertyThatWasLoaded) override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	void InvalidateCache();

private:
	UPROPERTY(Transient, VisibleAnywhere, Category = "Assembly")
	FString CoreAssemblyName = ZSHARP_CORE_ASSEMBLY_NAME;

	UPROPERTY(Transient, VisibleAnywhere, Category = "Assembly")
	FString EngineCoreAssemblyName = ZSHARP_CORE_ENGINE_ASSEMBLY_NAME;
	
	UPROPERTY(Transient, VisibleAnywhere, Category = "Assembly")
	FString EngineAssemblyName = ZSHARP_ENGINE_ASSEMBLY_NAME;
	
	UPROPERTY(Config, EditAnywhere, Category = "Mapping")
	TArray<FZModuleMappingContext> ModuleMappings;

	UPROPERTY(Transient, VisibleAnywhere, Category = "Mapping")
	TArray<FZModuleMappingContext> IntrinsicModuleMappings;

	UPROPERTY(Config, EditAnywhere, Category = "Field")
	TArray<FZFieldNameRedirector> FieldNameRedirectors;

	UPROPERTY(Transient, VisibleAnywhere, Category = "Field")
	TArray<FZFieldNameRedirector> IntrinsicFieldNameRedirectors;

	UPROPERTY(Config, EditAnywhere, Category = "Master ALC")
	TArray<FZMasterAlcStartupAssembly> MasterAlcStartupAssemblies;

private:
	TMap<FName, FZModuleMappingContext> ModuleMappingHash;
	TMap<FName, FZFieldNameRedirector> FieldNameRedirectorHash;
	
};


