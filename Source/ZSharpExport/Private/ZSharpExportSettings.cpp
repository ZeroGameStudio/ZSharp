// Copyright Zero Games. All Rights Reserved.


#include "ZSharpExportSettings.h"

UZSharpExportSettings::UZSharpExportSettings()
{
	IntrinsicModuleMappings.Emplace("Core", EngineAssemblyName);
	IntrinsicModuleMappings.Emplace("CoreUObject", EngineAssemblyName);
	IntrinsicModuleMappings.Emplace("FieldNotification", EngineAssemblyName);

	IntrinsicModuleMappings.Emplace("PhysicsCore", EngineAssemblyName);
	IntrinsicModuleMappings.Emplace("InputCore", EngineAssemblyName);

	IntrinsicModuleMappings.Emplace("Engine", EngineAssemblyName);
	IntrinsicModuleMappings.Emplace("EnhancedInput", EngineAssemblyName);

	IntrinsicModuleMappings.Emplace("ZSharpRuntime", EngineAssemblyName);

	IntrinsicFieldNameRedirectors.Emplace("/Script/CoreUObject.Object", "UnrealObject");

	IntrinsicFieldNameRedirectors.Emplace("/Script/CoreUObject.Field", "UnrealField");
	IntrinsicFieldNameRedirectors.Emplace("/Script/CoreUObject.Struct", "UnrealStruct");
	IntrinsicFieldNameRedirectors.Emplace("/Script/CoreUObject.Class", "UnrealClass");
	IntrinsicFieldNameRedirectors.Emplace("/Script/CoreUObject.ScriptStruct", "UnrealScriptStruct");
	IntrinsicFieldNameRedirectors.Emplace("/Script/CoreUObject.Enum", "UnrealEnum");
	IntrinsicFieldNameRedirectors.Emplace("/Script/CoreUObject.Interface", "UnrealInterface");

	IntrinsicFieldNameRedirectors.Emplace("/Script/CoreUObject.Function", "UnrealFunction");
	IntrinsicFieldNameRedirectors.Emplace("/Script/CoreUObject.Property", "UnrealProperty");

	IntrinsicFieldNameRedirectors.Emplace("/Script/CoreUObject.Guid", "UnrealGuid");
	IntrinsicFieldNameRedirectors.Emplace("/Script/CoreUObject.DateTime", "UnrealDateTime");
}

const FZModuleMappingContext* UZSharpExportSettings::GetModuleMappingContext(const FString& module) const
{
	return ModuleMappingHash.Find(FName { module });
}

void UZSharpExportSettings::ForeachMappedModule(TFunctionRef<void(const FString&, const FZModuleMappingContext&)> action) const
{
	for (const auto& pair : ModuleMappingHash)
	{
		action(pair.Key.ToString(), pair.Value);
	}
}

FString UZSharpExportSettings::RedirectFieldName(const FString& sourcePath) const
{
	const FZFieldNameRedirector* redirector = FieldNameRedirectorHash.Find(FName { sourcePath });
	return redirector ? redirector->TargetName : FString{};
}

TArray<FString> UZSharpExportSettings::GetModuleOptions()
{
	const auto cdo = GetDefault<UZSharpExportSettings>();
	
	TArray<FModuleStatus> statuses;
	FModuleManager::Get().QueryModules(statuses);

	TArray<FString> moduleNames;
	for (const auto& status : statuses)
	{
		if (cdo->GetModuleMappingContext(status.Name))
		{
			continue;
		}
		
		moduleNames.Emplace(status.Name);
	}

	moduleNames.StableSort();

	return moduleNames;
}

void UZSharpExportSettings::PostInitProperties()
{
	Super::PostInitProperties();

	InvalidateCache();
}

void UZSharpExportSettings::PostReloadConfig(FProperty* reloadedProperty)
{
	Super::PostReloadConfig(reloadedProperty);
	
	InvalidateCache();
}

#if WITH_EDITOR

void UZSharpExportSettings::PostEditChangeProperty(FPropertyChangedEvent& event)
{
	Super::PostEditChangeProperty(event);
	
	InvalidateCache();
}

#endif

void UZSharpExportSettings::InvalidateCache()
{
	ModuleMappingHash.Reset();
	for (const auto& ctx : ModuleMappings)
	{
		ModuleMappingHash.Emplace(FName { ctx.ModuleName }, ctx);
	}

	for (const auto& ctx : IntrinsicModuleMappings)
	{
		ModuleMappingHash.Emplace(FName { ctx.ModuleName }, ctx);
	}

	FieldNameRedirectorHash.Reset();
	for (const auto& redirector : FieldNameRedirectors)
	{
		FieldNameRedirectorHash.Emplace(FName { redirector.SourcePath }, redirector);
	}

	for (const auto& redirector : IntrinsicFieldNameRedirectors)
	{
		FieldNameRedirectorHash.Emplace(FName { redirector.SourcePath }, redirector);
	}
	
	ForceExportFieldPathsHash.Reset();
	for (const auto& path : ForceExportFieldPaths)
	{
		ForceExportFieldPathsHash.Add(path);
	}
	
	ForceNotExportFieldPathsHash.Reset();
	for (const auto& path : ForceNotExportFieldPaths)
	{
		ForceNotExportFieldPathsHash.Add(path);
	}
}


