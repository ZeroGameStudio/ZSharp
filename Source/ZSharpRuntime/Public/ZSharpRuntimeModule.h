// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

class IZSharpRuntimeModule : public IModuleInterface
{
public:
	static FORCEINLINE IZSharpRuntimeModule& Get()
	{
		static IZSharpRuntimeModule& Singleton = FModuleManager::LoadModuleChecked<IZSharpRuntimeModule>("ZSharpRuntime");
		return Singleton;
	}

	static FORCEINLINE bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("ZSharpRuntime");
	}
};
