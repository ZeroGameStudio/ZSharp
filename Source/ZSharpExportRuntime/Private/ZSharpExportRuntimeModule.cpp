// Copyright Zero Games. All Rights Reserved.

#include "ZSharpExportRuntimeModule.h"

class FZSharpExportRuntimeModule : public IZSharpExportRuntimeModule
{
	// Begin IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// End IModuleInterface
};

IMPLEMENT_MODULE(FZSharpExportRuntimeModule, ZSharpExportRuntime)

void FZSharpExportRuntimeModule::StartupModule()
{
}

void FZSharpExportRuntimeModule::ShutdownModule()
{
}
