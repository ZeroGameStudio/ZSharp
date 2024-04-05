// Copyright Zero Games. All Rights Reserved.

#include "ZSharpRuntimeModule.h"

class FZSharpRuntimeModule : public IZSharpRuntimeModule
{
	// Begin IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// End IModuleInterface
};

IMPLEMENT_MODULE(FZSharpRuntimeModule, ZSharpRuntime)

void FZSharpRuntimeModule::StartupModule()
{
}

void FZSharpRuntimeModule::ShutdownModule()
{
}
