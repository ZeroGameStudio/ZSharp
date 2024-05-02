// Copyright Zero Games. All Rights Reserved.

#include "ZSharpRuntimeModule.h"

#include "ZConjugateRegistry.h"
#include "CLR/IZSharpClr.h"
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

	void CreateMasterAlc();
	void UnloadMasterAlc();

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
	CreateMasterAlc();
#endif
}

void FZSharpRuntimeModule::ShutdownModule()
{
#if WITH_EDITOR
	FEditorDelegates::PreBeginPIE.RemoveAll(this);
	FEditorDelegates::ShutdownPIE.RemoveAll(this);
#else
	UnloadMasterAlc();
#endif
}

void FZSharpRuntimeModule::CreateMasterAlc()
{
	ZSharp::IZMasterAssemblyLoadContext* alc = ZSharp::IZSharpClr::Get().CreateMasterAlc();
	alc->RegisterZCallResolver(new ZSharp::FZCallResolver_Export{}, 0);
	alc->RegisterZCallResolver(new ZSharp::FZCallResolver_UFunction{}, 1);
	alc->LoadAssembly(FPaths::Combine(FPaths::ProjectDir(), "Binaries", "Managed", "ZeroGames.ZSharp.UnrealEngine.dll"));
}

void FZSharpRuntimeModule::UnloadMasterAlc()
{
	ZSharp::IZSharpClr::Get().GetMasterAlc()->Unload();
}

void FZSharpRuntimeModule::HandleBeginPIE(const bool bSimulating)
{
	CreateMasterAlc();
}

void FZSharpRuntimeModule::HandleEndPIE(const bool bSimulating)
{
	UnloadMasterAlc();
}


