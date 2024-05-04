// Copyright Zero Games. All Rights Reserved.


#include "ZSharpRuntimeSettings.h"

UZSharpRuntimeSettings::UZSharpRuntimeSettings()
{
	IntrinsicModuleAssemblyMapping.Emplace("Core", EngineAssemblyName);
	IntrinsicModuleAssemblyMapping.Emplace("CoreUObject", EngineAssemblyName);
	IntrinsicModuleAssemblyMapping.Emplace("Engine", EngineAssemblyName);
}


