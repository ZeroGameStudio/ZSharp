// Copyright Zero Games. All Rights Reserved.

#include "ZSharpCLRModule.h"

#include "CLR/CLRImplType.h"
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
	((FZCLRImplType&)ZSharp::IZCLR::Get()).Startup();
}

void FZSharpCLRModule::ShutdownModule()
{
	if (ZSharp::IZMasterAssemblyLoadContext* alc = ZSharp::IZCLR::Get().GetMasterALC())
	{
		alc->Unload();
	}
}


