// Copyright Zero Games. All Rights Reserved.


#include "ZSharpRuntimeSettings.h"

UZSharpRuntimeSettings::UZSharpRuntimeSettings()
{
	IntrinsicModuleMappings.Emplace("Core", EngineAssemblyName, false);
	IntrinsicModuleMappings.Emplace("CoreUObject", EngineAssemblyName, false);
	IntrinsicModuleMappings.Emplace("FieldNotification", EngineAssemblyName, false);

	IntrinsicModuleMappings.Emplace("PhysicsCore", EngineAssemblyName, false);
	IntrinsicModuleMappings.Emplace("InputCore", EngineAssemblyName, false);

	IntrinsicModuleMappings.Emplace("Engine", EngineAssemblyName, false);

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

const FZModuleMappingContext* UZSharpRuntimeSettings::GetModuleMappingContext(const FString& module) const
{
	return ModuleMappingHash.Find(FName { module });
}

void UZSharpRuntimeSettings::ForeachMappedModule(TFunctionRef<void(const FString&, const FZModuleMappingContext&)> action) const
{
	for (const auto& pair : ModuleMappingHash)
	{
		action(pair.Key.ToString(), pair.Value);
	}
}

FString UZSharpRuntimeSettings::GetFieldAlias(const FString& path) const
{
	const FZFieldNameRedirector* redirector = FieldNameRedirectorHash.Find(FName { path });
	return redirector ? redirector->TargetName : FString{};
}

void UZSharpRuntimeSettings::ForeachMasterAlcStartupAssembly(TFunctionRef<void(const FZMasterAlcStartupAssembly&)> action) const
{
	for (const auto& assembly : MasterAlcStartupAssemblies)
	{
		action(assembly);
	}
}

#if WITH_EDITOR

TArray<FString> UZSharpRuntimeSettings::GetModuleOptions()
{
	const auto cdo = GetDefault<UZSharpRuntimeSettings>();
	
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

#endif

void UZSharpRuntimeSettings::PostInitProperties()
{
	Super::PostInitProperties();

	InvalidateCache();
}

void UZSharpRuntimeSettings::PostReloadConfig(FProperty* PropertyThatWasLoaded)
{
	Super::PostReloadConfig(PropertyThatWasLoaded);

	InvalidateCache();
}

#if WITH_EDITOR

void UZSharpRuntimeSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	InvalidateCache();
}

#endif

void UZSharpRuntimeSettings::InvalidateCache()
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
		FieldNameRedirectorHash.Emplace(FName { redirector.SourceName }, redirector);
	}

	for (const auto& redirector : IntrinsicFieldNameRedirectors)
	{
		FieldNameRedirectorHash.Emplace(FName { redirector.SourceName }, redirector);
	}
}


