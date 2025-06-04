// Copyright Zero Games. All Rights Reserved.

#pragma once

class IZSharpRuntimeModule : public IModuleInterface
{
	
public:
	static FORCEINLINE IZSharpRuntimeModule& Get()
	{
		static IZSharpRuntimeModule& GInstance = FModuleManager::LoadModuleChecked<IZSharpRuntimeModule>("ZSharpRuntime");
		return GInstance;
	}

	static FORCEINLINE bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("ZSharpRuntime");
	}

public:
	virtual bool HasEngineAssembly() const = 0;
	
};


