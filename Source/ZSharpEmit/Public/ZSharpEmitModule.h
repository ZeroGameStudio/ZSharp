// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

class IZSharpEmitModule : public IModuleInterface
{
public:
	static FORCEINLINE IZSharpEmitModule& Get()
	{
		static IZSharpEmitModule& GSingleton = FModuleManager::LoadModuleChecked<IZSharpEmitModule>("ZSharpEmit");
		return GSingleton;
	}

	static FORCEINLINE bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("ZSharpEmit");
	}
};


