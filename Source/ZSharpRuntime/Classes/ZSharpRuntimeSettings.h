﻿// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "ZSharpRuntimeSettings.generated.h"

UENUM(meta = (ZSharpNoExport))
enum class EZEmitVirtualModuleLoadingPhase : uint8
{
	EarliestPossible,
	PostEngineInit,
	BeforeModule,
	AfterModule,
};

USTRUCT(meta = (ZSharpNoExport))
struct FZEmitVirtualModule
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString ModuleName;

	UPROPERTY(EditAnywhere)
	EZEmitVirtualModuleLoadingPhase LoadingPhase = EZEmitVirtualModuleLoadingPhase::PostEngineInit;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "LoadingPhase > EZEmitVirtualModuleLoadingPhase::PostEngineInit", EditConditionHides))
	FString TargetModule;
	
};

USTRUCT(meta = (ZSharpNoExport))
struct FZModuleEmitMetadataSource
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (GetOptions = "ZSharpRuntime.ZSharpRuntimeSettings.GetModuleOptions"))
	FString ModuleName;

	UPROPERTY(EditAnywhere)
	FString AssemblyName;
};

UCLASS(Config = ZSharp, DefaultConfig, meta = (ZSharpNoExport))
class ZSHARPRUNTIME_API UZSharpRuntimeSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	virtual FName GetCategoryName() const override { return TEXT("ZSharp"); }
	
public:
	void ForEachEmitVirtualModule(TFunctionRef<void(const FZEmitVirtualModule&)> action) const;
	int32 GetModuleEmitMetadataSources(const FString& moduleName, TArray<FZModuleEmitMetadataSource>& result) const;

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
	UPROPERTY(Config, EditAnywhere, Category = "Emit", meta = (ConfigRestartRequired = true))
	TArray<FZEmitVirtualModule> EmitVirtualModules;
	
	UPROPERTY(Config, EditAnywhere, Category = "Emit", meta = (ConfigRestartRequired = true))
	TArray<FZModuleEmitMetadataSource> ModuleEmitMetadataSources;

private:
	TMultiMap<FName, FZModuleEmitMetadataSource> ModuleEmitMetadataSourceLookup;
	
};


