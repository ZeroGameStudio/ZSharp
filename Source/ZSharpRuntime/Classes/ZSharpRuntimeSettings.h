// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "ZSharpRuntimeSettings.generated.h"

USTRUCT(meta = (ZSharpNoExport))
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

USTRUCT(meta = (ZSharpNoExport))
struct FZFieldNameRedirector
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString SourcePath;

	UPROPERTY(EditAnywhere)
	FString TargetName;
	
};

/**
 * 
 */
UCLASS(Config = ZSharp, DefaultConfig, meta = (ZSharpNoExport))
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

	FString RedirectFieldName(const FString& sourcePath) const;

#if WITH_EDITOR
private:
	UFUNCTION()
	static TArray<FString> GetModuleOptions();
#endif

private:
	virtual void PostInitProperties() override;
	virtual void PostReloadConfig(FProperty* reloadedProperty) override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& event) override;
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

private:
	TMap<FName, FZModuleMappingContext> ModuleMappingHash;
	TMap<FName, FZFieldNameRedirector> FieldNameRedirectorHash;
	
};


