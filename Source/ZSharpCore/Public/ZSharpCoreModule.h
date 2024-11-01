// Copyright Zero Games. All Rights Reserved.

#pragma once

/**
 * Module interface for ZSharpCore.
 */
class IZSharpCoreModule : public IModuleInterface
{
	
public:
	/**
	 * Gets the module singleton.
	 * 
	 * @return The module singleton.
	 */
	static FORCEINLINE IZSharpCoreModule& Get()
	{
		static IZSharpCoreModule& GInstance = FModuleManager::LoadModuleChecked<IZSharpCoreModule>("ZSharpCore");
		return GInstance;
	}

	/**
	 * Retrieves whether the module is available.
	 * 
	 * @return true if the module is available, otherwise false.
	 */
	static FORCEINLINE bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("ZSharpCore");
	}
	
};


