// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

class IZSharpEditorModule : public IModuleInterface
{
public:
	static FORCEINLINE IZSharpEditorModule& Get()
	{
		static IZSharpEditorModule& Singleton = FModuleManager::LoadModuleChecked<IZSharpEditorModule>("ZSharpEditor");
		return Singleton;
	}

	static FORCEINLINE bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("ZSharpEditor");
	}
};
