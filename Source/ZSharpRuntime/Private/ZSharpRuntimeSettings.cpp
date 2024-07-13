// Copyright Zero Games. All Rights Reserved.


#include "ZSharpRuntimeSettings.h"

UZSharpRuntimeSettings::UZSharpRuntimeSettings()
{
	IntrinsicModuleAssemblyMapping.Emplace("Core", EngineAssemblyName);
	IntrinsicModuleAssemblyMapping.Emplace("CoreUObject", EngineAssemblyName);
	
	IntrinsicModuleAssemblyMapping.Emplace("PhysicsCore", EngineAssemblyName);
	IntrinsicModuleAssemblyMapping.Emplace("InputCore", EngineAssemblyName);
	
	IntrinsicModuleAssemblyMapping.Emplace("Engine", EngineAssemblyName);
	
	IntrinsicModuleAssemblyMapping.Emplace("ZSharpRuntime", EngineAssemblyName);
}

bool UZSharpRuntimeSettings::IsModuleMapped(const FString& module) const
{
	return IntrinsicModuleAssemblyMapping.Contains(module) || ModuleAssemblyMapping.Contains(module);
}

bool UZSharpRuntimeSettings::TryGetModuleAssembly(const FString& module, FString& outAssembly) const
{
	const FString* assembly = IntrinsicModuleAssemblyMapping.Find(module);
	if (!assembly)
	{
		assembly = ModuleAssemblyMapping.Find(module);
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


