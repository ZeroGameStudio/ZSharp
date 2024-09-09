// Copyright Zero Games. All Rights Reserved.

#include "ZSharpEmitModule.h"

#include "Scan/ZUnrealFieldScanner.h"

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
	ZSharp::FZUnrealFieldScanner::Get().Startup();
}

void FZSharpEmitModule::ShutdownModule()
{
	ZSharp::FZUnrealFieldScanner::Get().Shutdown();
}


