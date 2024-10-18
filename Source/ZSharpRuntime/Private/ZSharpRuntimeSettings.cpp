// Copyright Zero Games. All Rights Reserved.


#include "ZSharpRuntimeSettings.h"

UZSharpRuntimeSettings::UZSharpRuntimeSettings()
{
	IntrinsicModuleAssemblyMapping.Emplace("Core", { EngineAssemblyName, false });
	IntrinsicModuleAssemblyMapping.Emplace("CoreUObject", { EngineAssemblyName, false });
	IntrinsicModuleAssemblyMapping.Emplace("FieldNotification", { EngineAssemblyName, false });
	
	IntrinsicModuleAssemblyMapping.Emplace("PhysicsCore", { EngineAssemblyName, false });
	IntrinsicModuleAssemblyMapping.Emplace("InputCore", { EngineAssemblyName, false });
	
	IntrinsicModuleAssemblyMapping.Emplace("Engine", { EngineAssemblyName, false });

	IntrinsicFieldAliasMap.Emplace("/Script/CoreUObject.Object", "UnrealObject");

	IntrinsicFieldAliasMap.Emplace("/Script/CoreUObject.Field", "UnrealField");
	IntrinsicFieldAliasMap.Emplace("/Script/CoreUObject.Struct", "UnrealStruct");
	IntrinsicFieldAliasMap.Emplace("/Script/CoreUObject.Class", "UnrealClass");
	IntrinsicFieldAliasMap.Emplace("/Script/CoreUObject.ScriptStruct", "UnrealScriptStruct");
	IntrinsicFieldAliasMap.Emplace("/Script/CoreUObject.Enum", "UnrealEnum");
	IntrinsicFieldAliasMap.Emplace("/Script/CoreUObject.Interface", "UnrealInterface");

	IntrinsicFieldAliasMap.Emplace("/Script/CoreUObject.Function", "UnrealFunction");
	IntrinsicFieldAliasMap.Emplace("/Script/CoreUObject.Property", "UnrealProperty");

	IntrinsicFieldAliasMap.Emplace("/Script/CoreUObject.Guid", "UnrealGuid");
	IntrinsicFieldAliasMap.Emplace("/Script/CoreUObject.DateTime", "UnrealDateTime");

	// There are some delegates in UWidget with bad name which often cause naming conflicts.
	IntrinsicFieldAliasMap.Emplace("/Script/UMG.Widget:GetWidget__DelegateSignature", "GetWidgetDelegate__DelegateSignature");
	IntrinsicFieldAliasMap.Emplace("/Script/UMG.Widget:GetText__DelegateSignature", "GetTextDelegate__DelegateSignature");
	IntrinsicFieldAliasMap.Emplace("/Script/UMG.Widget:GetInt32__DelegateSignature", "GetInt32Delegate__DelegateSignature");
	IntrinsicFieldAliasMap.Emplace("/Script/UMG.Widget:GetFloat__DelegateSignature", "GetFloatDelegate__DelegateSignature");
	IntrinsicFieldAliasMap.Emplace("/Script/UMG.Widget:GetBool__DelegateSignature", "GetBoolDelegate__DelegateSignature");
	IntrinsicFieldAliasMap.Emplace("/Script/UMG.Widget:GetSlateColor__DelegateSignature", "GetSlateColorDelegate__DelegateSignature");
	IntrinsicFieldAliasMap.Emplace("/Script/UMG.Widget:GetLinearColor__DelegateSignature", "GetLinearColorDelegate__DelegateSignature");
	IntrinsicFieldAliasMap.Emplace("/Script/UMG.Widget:GetSlateBrush__DelegateSignature", "GetSlateBrushDelegate__DelegateSignature");
	IntrinsicFieldAliasMap.Emplace("/Script/UMG.Widget:GetSlateVisibility__DelegateSignature", "GetSlateVisibilityDelegate__DelegateSignature");
	IntrinsicFieldAliasMap.Emplace("/Script/UMG.Widget:GetMouseCursor__DelegateSignature", "GetMouseCursorDelegate__DelegateSignature");
	IntrinsicFieldAliasMap.Emplace("/Script/UMG.Widget:GetCheckBoxState__DelegateSignature", "GetCheckBoxStateDelegate__DelegateSignature");
}

const FZModuleMappingContext* UZSharpRuntimeSettings::GetModuleMappingContext(const FString& module) const
{
	const FZModuleMappingContext* ctx = IntrinsicModuleAssemblyMapping.Find(module);
	if (!ctx)
	{
		ctx = ModuleAssemblyMapping.Find(module);
	}

	return ctx;
}

void UZSharpRuntimeSettings::ForeachMappedModule(TFunctionRef<void(const FString&, const FZModuleMappingContext&)> action) const
{
	for (const auto& pair : IntrinsicModuleAssemblyMapping)
	{
		action(pair.Key, pair.Value);
	}

	for (const auto& pair : ModuleAssemblyMapping)
	{
		action(pair.Key, pair.Value);
	}
}

FString UZSharpRuntimeSettings::GetFieldAlias(const FString& path) const
{
	const FString* alias = IntrinsicFieldAliasMap.Find(path);
	if (!alias)
	{
		alias = FieldAliasMap.Find(path);
	}

	return alias ? *alias : FString{};
}

#if WITH_EDITOR

TArray<FString> UZSharpRuntimeSettings::GetModuleOptions()
{
	TArray<FModuleStatus> statuses;
	FModuleManager::Get().QueryModules(statuses);

	TArray<FString> moduleNames;
	for (const auto& status : statuses)
	{
		moduleNames.Emplace(status.Name);
	}

	moduleNames.StableSort();

	return moduleNames;
}

#endif


