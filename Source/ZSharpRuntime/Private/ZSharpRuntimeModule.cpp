// Copyright Zero Games. All Rights Reserved.

#include "ZSharpRuntimeModule.h"

#include "CLR/IZCLR.h"
#include "Interop/IZAssembly.h"
#include "Interop/IZMasterAssemblyLoadContext.h"

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
	ZSharp::IZMasterAssemblyLoadContext* alc = ZSharp::IZCLR::Get().CreateMasterALC();

	FString path = FPaths::Combine(FPaths::ProjectDir(), "Binaries", "Managed", "Game.dll");
	TArray<uint8> content;
	FFileHelper::LoadFileToArray(content, *path, FILEREAD_Silent);
	ZSharp::IZAssembly* assembly = alc->LoadAssembly(content);
}

void FZSharpRuntimeModule::ShutdownModule()
{

}
