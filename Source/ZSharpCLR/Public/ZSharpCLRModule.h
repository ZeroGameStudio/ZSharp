// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

class IZSharpCLRModule : public IModuleInterface
{
public:
	static FORCEINLINE IZSharpCLRModule& Get()
	{
		static IZSharpCLRModule& GSingleton = FModuleManager::LoadModuleChecked<IZSharpCLRModule>("ZSharpCLR");
		return GSingleton;
	}

	static FORCEINLINE bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("ZSharpCLR");
	}
};


