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
		static IZSharpEditorModule& GInstance = FModuleManager::LoadModuleChecked<IZSharpEditorModule>("ZSharpEditor");
		return GInstance;
	}

	static FORCEINLINE bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("ZSharpEditor");
	}
};
