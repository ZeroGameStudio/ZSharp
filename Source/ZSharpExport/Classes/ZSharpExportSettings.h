// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSharpExportSettings.generated.h"

USTRUCT()
struct FZModuleMappingContext
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (GetOptions = "ZSharpExport.ZSharpExportSettings.GetModuleOptions"))
	FString ModuleName;

	UPROPERTY(EditAnywhere)
	FString AssemblyName;
};

USTRUCT()
struct FZFieldNameRedirector
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString SourcePath;

	UPROPERTY(EditAnywhere)
	FString TargetName;
};

UCLASS(Config = ZSharp, DefaultConfig)
class ZSHARPEXPORT_API UZSharpExportSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UZSharpExportSettings();

public:
	virtual FName GetContainerName() const override { return TEXT("Editor"); }
	virtual FName GetCategoryName() const override { return TEXT("ZSharp"); }

public:
	const FZModuleMappingContext* GetModuleMappingContext(const FString& module) const;
	void ForeachMappedModule(TFunctionRef<void(const FString&, const FZModuleMappingContext&)> action) const;

	FString RedirectFieldName(const FString& sourcePath) const;
	
	bool ShouldExportDeprecatedFields() const { return bExportDeprecatedFields; }
	bool ShouldTreatDeprecatedPostfixAsDeprecated() const { return bTreatDeprecatedPostfixAsDeprecated; }
	bool ShouldExportEditorOnlyFields() const { return bExportEditorOnlyFields; }
	bool IsForceExportFieldPath(const FString& path) const { return ForceExportFieldPathsLookup.Contains(path); }
	bool IsForceNotExportFieldPath(const FString& path) const { return ForceNotExportFieldPathsLookup.Contains(path); }

	bool ShouldUseLooseDefaultParameterName() const { return bUseLooseDefaultParameterName; }

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
	
	UPROPERTY(Config, EditAnywhere, Category = "Export")
	bool bExportDeprecatedFields = false;

	UPROPERTY(Config, EditAnywhere, Category = "Export", meta = (EditCondition = "!bExportDeprecatedFields", EditConditionHides))
	bool bTreatDeprecatedPostfixAsDeprecated = true;
	
	UPROPERTY(Config, EditAnywhere, Category = "Export")
	bool bExportEditorOnlyFields = false;

	UPROPERTY(Config, EditAnywhere, Category = "Export")
	TArray<FString> ForceExportFieldPaths;

	UPROPERTY(Config, EditAnywhere, Category = "Export")
	TArray<FString> ForceNotExportFieldPaths;

	UPROPERTY(Config, EditAnywhere, Category = "Export|Function")
	bool bUseLooseDefaultParameterName = false;

private:
	TMap<FName, FZModuleMappingContext> ModuleMappingLookup;
	TMap<FName, FZFieldNameRedirector> FieldNameRedirectorLookup;
	TSet<FString> ForceExportFieldPathsLookup;
	TSet<FString> ForceNotExportFieldPathsLookup;
	
};


