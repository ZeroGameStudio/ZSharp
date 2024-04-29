// Copyright Zero Games. All Rights Reserved.

#include "ZSharpRuntimeModule.h"

#include "ZConjugateRegistry.h"
#include "CLR/IZSharpCLR.h"
#include "ALC/IZMasterAssemblyLoadContext.h"
#include "ZCall/ZCallResolver_Export.h"
#include "ZCall/ZCallResolver_UFunction.h"

class FZSharpRuntimeModule : public IZSharpRuntimeModule
{

	using ThisClass = FZSharpRuntimeModule;
	
	// Begin IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// End IModuleInterface

	void CreateMasterALC();
	void UnloadMasterALC();

#if WITH_EDITOR
	void HandleBeginPIE(const bool bSimulating);
	void HandleEndPIE(const bool bSimulating);
#endif
	
};

IMPLEMENT_MODULE(FZSharpRuntimeModule, ZSharpRuntime)

void FZSharpRuntimeModule::StartupModule()
{
	ZSharp::FZConjugateRegistry::Startup();

#if WITH_EDITOR
	FEditorDelegates::PreBeginPIE.AddRaw(this, &ThisClass::HandleBeginPIE);
	FEditorDelegates::ShutdownPIE.AddRaw(this, &ThisClass::HandleEndPIE);
#else
	CreateMasterALC();
#endif
}

void FZSharpRuntimeModule::ShutdownModule()
{
#if WITH_EDITOR
	FEditorDelegates::PreBeginPIE.RemoveAll(this);
	FEditorDelegates::ShutdownPIE.RemoveAll(this);
#else
	UnloadMasterALC();
#endif
}

void FZSharpRuntimeModule::CreateMasterALC()
{
	ZSharp::IZMasterAssemblyLoadContext* alc = ZSharp::IZSharpCLR::Get().CreateMasterALC();
	alc->RegisterZCallResolver(new ZSharp::FZCallResolver_Export{}, 0);
	alc->RegisterZCallResolver(new ZSharp::FZCallResolver_UFunction{}, 1);
	alc->LoadAssembly(FPaths::Combine(FPaths::ProjectPluginsDir(), "ZeroGames", "ZSharp", "Binaries", "Managed", "ZeroGames.ZSharp.UnrealEngine.dll"));
}

void FZSharpRuntimeModule::UnloadMasterALC()
{
	ZSharp::IZSharpCLR::Get().GetMasterALC()->Unload();
}

void FZSharpRuntimeModule::HandleBeginPIE(const bool bSimulating)
{
	CreateMasterALC();
}

void FZSharpRuntimeModule::HandleEndPIE(const bool bSimulating)
{
	UnloadMasterALC();
}


