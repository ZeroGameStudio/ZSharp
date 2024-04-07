// Copyright Zero Games. All Rights Reserved.

#include "ZSharpCLRModule.h"

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
}

void FZSharpCLRModule::ShutdownModule()
{
}


