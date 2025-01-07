// Copyright Zero Games. All Rights Reserved.

#pragma once

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


