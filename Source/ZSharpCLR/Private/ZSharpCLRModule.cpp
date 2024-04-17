// Copyright Zero Games. All Rights Reserved.

#include "ZSharpCLRModule.h"

#include "CLR\ZSharpCLR.h"
#include "CLR/IZSharpCLR.h"
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
	FZSharpCLR::Get().Startup();
}

void FZSharpCLRModule::ShutdownModule()
{
	if (ZSharp::IZMasterAssemblyLoadContext* alc = ZSharp::IZSharpCLR::Get().GetMasterALC())
	{
		alc->Unload();
	}
}


