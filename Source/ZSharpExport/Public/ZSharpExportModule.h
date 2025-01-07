// Copyright Zero Games. All Rights Reserved.

#pragma once

class IZSharpExportModule : public IModuleInterface
{
public:
	static FORCEINLINE IZSharpExportModule& Get()
	{
		static IZSharpExportModule& GInstance = FModuleManager::LoadModuleChecked<IZSharpExportModule>("ZSharpExport");
		return GInstance;
	}

	static FORCEINLINE bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("ZSharpExport");
	}
};


