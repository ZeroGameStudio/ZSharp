// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSharpExportSettings.generated.h"

/**
 * 
 */
UCLASS(Config = ZSharpExport, DefaultConfig)
class ZSHARPEXPORT_API UZSharpExportSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UZSharpExportSettings();

public:
	virtual FName GetContainerName() const override { return TEXT("Editor"); }
	virtual FName GetCategoryName() const override { return TEXT("ZSharp"); }

public:
	bool ShouldExportDeprecatedFields() const { return bExportDeprecatedFields; }
	bool ShouldTreatDeprecatedPostfixAsDeprecated() const { return bTreatDeprecatedPostfixAsDeprecated; }
	bool ShouldExportEditorOnlyFields() const { return bExportEditorOnlyFields; }
	bool IsForceExportFieldPath(const FString& path) const { return ForceExportFieldPathsHash.Contains(path); }
	bool IsForceNotExportFieldPath(const FString& path) const { return IntrinsicForceNotExportFieldPaths.Contains(path) || ForceNotExportFieldPathsHash.Contains(path); }

private:
	virtual void PostInitProperties() override;
	virtual void PostReloadConfig(FProperty* PropertyThatWasLoaded) override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	void InvalidateCache();

private:
	UPROPERTY(Config, EditAnywhere, Category = "Export")
	bool bExportDeprecatedFields;

	UPROPERTY(Config, EditAnywhere, Category = "Export", meta = (EditCondition = "!bExportDeprecatedFields", EditConditionHides))
	bool bTreatDeprecatedPostfixAsDeprecated;
	
	UPROPERTY(Config, EditAnywhere, Category = "Export")
	bool bExportEditorOnlyFields;

	UPROPERTY(Config, EditAnywhere, Category = "Export")
	TArray<FString> ForceExportFieldPaths;

	UPROPERTY(Config, EditAnywhere, Category = "Export")
	TArray<FString> ForceNotExportFieldPaths;

	UPROPERTY(Transient, VisibleAnywhere, Category = "Export")
	TSet<FString> IntrinsicForceNotExportFieldPaths;

private:
	TSet<FString> ForceExportFieldPathsHash;
	TSet<FString> ForceNotExportFieldPathsHash;
	
};


