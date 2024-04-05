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
		static IZSharpCLRModule& Singleton = FModuleManager::LoadModuleChecked<IZSharpCLRModule>("ZSharpCLR");
		return Singleton;
	}

	static FORCEINLINE bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("ZSharpCLR");
	}
};
