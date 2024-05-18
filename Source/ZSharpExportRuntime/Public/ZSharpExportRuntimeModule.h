// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

class IZSharpExportRuntimeModule : public IModuleInterface
{
public:
	static FORCEINLINE IZSharpExportRuntimeModule& Get()
	{
		static IZSharpExportRuntimeModule& Singleton = FModuleManager::LoadModuleChecked<IZSharpExportRuntimeModule>("ZSharpExportRuntime");
		return Singleton;
	}

	static FORCEINLINE bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("ZSharpExportRuntime");
	}
};
