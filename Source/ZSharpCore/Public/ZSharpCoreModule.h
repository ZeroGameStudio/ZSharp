// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

class IZSharpCoreModule : public IModuleInterface
{
public:
	static FORCEINLINE IZSharpCoreModule& Get()
	{
		static IZSharpCoreModule& GSingleton = FModuleManager::LoadModuleChecked<IZSharpCoreModule>("ZSharpCore");
		return GSingleton;
	}

	static FORCEINLINE bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("ZSharpCore");
	}
};


