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
	return ModuleMappingLookup.Find(FName { module });
}

void UZSharpExportSettings::ForeachMappedModule(TFunctionRef<void(const FString&, const FZModuleMappingContext&)> action) const
{
	for (const auto& pair : ModuleMappingLookup)
	{
		action(pair.Key.ToString(), pair.Value);
	}
}

FString UZSharpExportSettings::RedirectFieldName(const FString& sourcePath) const
{
	const FZFieldNameRedirector* redirector = FieldNameRedirectorLookup.Find(FName { sourcePath });
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
	ModuleMappingLookup.Reset();
	for (const auto& ctx : ModuleMappings)
	{
		ModuleMappingLookup.Emplace(FName { ctx.ModuleName }, ctx);
	}

	for (const auto& ctx : IntrinsicModuleMappings)
	{
		ModuleMappingLookup.Emplace(FName { ctx.ModuleName }, ctx);
	}

	FieldNameRedirectorLookup.Reset();
	for (const auto& redirector : FieldNameRedirectors)
	{
		FieldNameRedirectorLookup.Emplace(FName { redirector.SourcePath }, redirector);
	}

	for (const auto& redirector : IntrinsicFieldNameRedirectors)
	{
		FieldNameRedirectorLookup.Emplace(FName { redirector.SourcePath }, redirector);
	}
	
	ForceExportFieldPathsLookup.Reset();
	for (const auto& path : ForceExportFieldPaths)
	{
		ForceExportFieldPathsLookup.Add(path);
	}
	
	ForceNotExportFieldPathsLookup.Reset();
	for (const auto& path : ForceNotExportFieldPaths)
	{
		ForceNotExportFieldPathsLookup.Add(path);
	}
}


