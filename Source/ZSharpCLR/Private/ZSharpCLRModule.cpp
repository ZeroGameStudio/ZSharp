// Copyright Zero Games. All Rights Reserved.

#include "ZSharpCLRModule.h"

#include "CLR/CLR.h"

class FZSharpCLRModule : public IZSharpCLRModule
{
	// Begin IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// End IModuleInterface
};

IMPLEMENT_MODULE(FZSharpCLRModule, ZSharpCLR)

void FZSharpCLRModule::StartupModule()
{
	ZSharp::FCLR::Get().Startup();
}

void FZSharpCLRModule::ShutdownModule()
{
}
