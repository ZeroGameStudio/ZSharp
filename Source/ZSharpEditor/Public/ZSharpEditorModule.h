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
		static IZSharpEditorModule& GSingleton = FModuleManager::LoadModuleChecked<IZSharpEditorModule>("ZSharpEditor");
		return GSingleton;
	}

	static FORCEINLINE bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("ZSharpEditor");
	}
};
