// Copyright Zero Games. All Rights Reserved.


#include "ZSharpRuntimeSettings.h"

UZSharpRuntimeSettings::UZSharpRuntimeSettings()
{
	IntrinsicModuleAssemblyMapping.Emplace("Core", EngineAssemblyName);
	IntrinsicModuleAssemblyMapping.Emplace("CoreUObject", EngineAssemblyName);
	IntrinsicModuleAssemblyMapping.Emplace("Engine", EngineAssemblyName);
}

bool UZSharpRuntimeSettings::IsModuleMapped(const FString& module) const
{
	return ModuleAssemblyMapping.Contains(module) || IntrinsicModuleAssemblyMapping.Contains(module);
}

bool UZSharpRuntimeSettings::TryGetModuleAssembly(const FString& module, FString& outAssembly) const
{
	const FString* assembly = ModuleAssemblyMapping.Find(module);
	if (!assembly)
	{
		assembly = IntrinsicModuleAssemblyMapping.Find(module);
	}

	outAssembly = assembly ? *assembly : "";
	return !!assembly;
}

void UZSharpRuntimeSettings::ForeachMappedModule(TFunctionRef<void(const FString&, const FString&)> action) const
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


