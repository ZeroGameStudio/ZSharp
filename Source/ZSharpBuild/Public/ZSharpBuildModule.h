// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

class IZSharpBuildModule : public IModuleInterface
{
public:
	static FORCEINLINE IZSharpBuildModule& Get()
	{
		static IZSharpBuildModule& Singleton = FModuleManager::LoadModuleChecked<IZSharpBuildModule>("ZSharpBuild");
		return Singleton;
	}

	static FORCEINLINE bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("ZSharpBuild");
	}
};
