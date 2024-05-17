// Copyright Zero Games. All Rights Reserved.


#include "ZSharpExportSettings.h"

UZSharpExportSettings::UZSharpExportSettings()
{
	IntrinsicModuleAssemblyMapping.Emplace("Core", EngineAssemblyName);
	IntrinsicModuleAssemblyMapping.Emplace("CoreUObject", EngineAssemblyName);
	IntrinsicModuleAssemblyMapping.Emplace("Engine", EngineAssemblyName);
}

bool UZSharpExportSettings::IsModuleMapped(const FString& module) const
{
	return ModuleAssemblyMapping.Contains(module) || IntrinsicModuleAssemblyMapping.Contains(module);
}

bool UZSharpExportSettings::TryGetModuleAssembly(const FString& module, FString& outAssembly) const
{
	const FString* assembly = ModuleAssemblyMapping.Find(module);
	if (!assembly)
	{
		assembly = IntrinsicModuleAssemblyMapping.Find(module);
	}

	outAssembly = assembly ? *assembly : "";
	return !!assembly;
}

void UZSharpExportSettings::ForeachMappedModule(TFunctionRef<void(const FString&, const FString&)> action) const
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


