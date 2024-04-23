// Copyright Zero Games. All Rights Reserved.

#include "ZSharpRuntimeModule.h"

#include "CLR/IZSharpCLR.h"
#include "Interop/IZAssembly.h"
#include "Interop/IZMasterAssemblyLoadContext.h"
#include "ZCall/ZCallResolver_Export.h"
#include "ZCall/ZCallResolver_UFunction.h"

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
	ZSharp::IZMasterAssemblyLoadContext* alc = ZSharp::IZSharpCLR::Get().CreateMasterALC();
	alc->RegisterZCallResolver(new ZSharp::FZCallResolver_Export{}, 0);
	alc->RegisterZCallResolver(new ZSharp::FZCallResolver_UFunction{}, 1);

	{
		FString path = FPaths::Combine(FPaths::ProjectPluginsDir(), "ZeroGames", "ZSharp", "Binaries", "Managed", "ZeroGames.ZSharp.UnrealEngine.dll");
		TArray<uint8> content;
		FFileHelper::LoadFileToArray(content, *path, FILEREAD_Silent);
		const ZSharp::IZAssembly* assembly = alc->LoadAssembly(content);
	}

	{
		FString path = FPaths::Combine(FPaths::ProjectDir(), "Binaries", "Managed", "Game.dll");
		TArray<uint8> content;
		FFileHelper::LoadFileToArray(content, *path, FILEREAD_Silent);
		const ZSharp::IZAssembly* assembly = alc->LoadAssembly(content);
	}
}

void FZSharpRuntimeModule::ShutdownModule()
{

}
