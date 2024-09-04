// Copyright Zero Games. All Rights Reserved.

#include "ZSharpEmitModule.h"

class FZSharpEmitModule : public IZSharpEmitModule
{
	// Begin IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// End IModuleInterface
};

IMPLEMENT_MODULE(FZSharpEmitModule, ZSharpEmit)

void FZSharpEmitModule::StartupModule()
{
}

void FZSharpEmitModule::ShutdownModule()
{
}
