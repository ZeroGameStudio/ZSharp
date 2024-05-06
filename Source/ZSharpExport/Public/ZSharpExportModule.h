// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

class IZSharpExportModule : public IModuleInterface
{
public:
	static FORCEINLINE IZSharpExportModule& Get()
	{
		static IZSharpExportModule& Singleton = FModuleManager::LoadModuleChecked<IZSharpExportModule>("ZSharpExport");
		return Singleton;
	}

	static FORCEINLINE bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("ZSharpExport");
	}
};


