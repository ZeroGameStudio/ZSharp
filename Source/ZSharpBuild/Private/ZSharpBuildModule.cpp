// Copyright Zero Games. All Rights Reserved.

#include "ZSharpBuildModule.h"

class FZSharpBuildModule : public IZSharpBuildModule
{
	// Begin IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// End IModuleInterface
};

IMPLEMENT_MODULE(FZSharpBuildModule, ZSharpBuild)

void FZSharpBuildModule::StartupModule()
{
}

void FZSharpBuildModule::ShutdownModule()
{
}
