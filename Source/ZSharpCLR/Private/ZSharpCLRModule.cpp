// Copyright Zero Games. All Rights Reserved.

#include "ZSharpCLRModule.h"

#include "CLR/IZCLR.h"
#include "Interop/IZMasterAssemblyLoadContext.h"

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
	// Startup the runtime as soon as the module startup.
	(void)ZSharp::IZCLR::Get();
}

void FZSharpCLRModule::ShutdownModule()
{
	if (ZSharp::IZMasterAssemblyLoadContext* alc = ZSharp::IZCLR::Get().GetMasterALC())
	{
		alc->Unload();
	}
}


