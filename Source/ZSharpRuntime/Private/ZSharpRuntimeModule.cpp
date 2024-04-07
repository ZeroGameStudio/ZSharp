// Copyright Zero Games. All Rights Reserved.

#include "ZSharpRuntimeModule.h"

#include "CLR/IZCLR.h"

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
	ZSharp::IZCLR::Get().Startup();
}

void FZSharpRuntimeModule::ShutdownModule()
{
}
