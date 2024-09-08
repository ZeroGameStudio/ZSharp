// Copyright Zero Games. All Rights Reserved.


#include "ZSharpEmitSettings.h"

TArray<FString> UZSharpEmitSettings::GetModuleAssembliesToScan(const FName& moduleName) const
{
	const FZAssembliesToScan* assemblies = ModuleAssembliesToScanMap.Find(moduleName);
	if (!assemblies)
	{
		return {};
	}

	return assemblies->Assemblies;
}


