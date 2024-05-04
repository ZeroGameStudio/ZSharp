// Copyright Zero Games. All Rights Reserved.

#include "ZSharpExportModule.h"

class FZSharpExportModule : public IZSharpExportModule
{
	// Begin IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// End IModuleInterface
};

IMPLEMENT_MODULE(FZSharpExportModule, ZSharpExport)

void FZSharpExportModule::StartupModule()
{
}

void FZSharpExportModule::ShutdownModule()
{
}
