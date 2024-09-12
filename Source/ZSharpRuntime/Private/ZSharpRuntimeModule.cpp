// Copyright Zero Games. All Rights Reserved.

#include "ZSharpRuntimeModule.h"

#include "CLR/IZSharpClr.h"
#include "ALC/IZMasterAssemblyLoadContext.h"
#include "Emit/ZUnrealFieldScanner.h"
#include "ZCall/ZCallResolver_Export.h"
#include "ZCall/ZCallResolver_UFunction.h"
#include "ZCall/ZCallResolver_UProperty.h"

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
	void HandleBeginPIE(const bool simulating);
	void HandleEndPIE(const bool simulating);
#endif
	
};

IMPLEMENT_MODULE(FZSharpRuntimeModule, ZSharpRuntime)

void FZSharpRuntimeModule::StartupModule()
{
	ZSharp::FZUnrealFieldScanner::Get().Startup();
	
#if WITH_EDITOR
	FEditorDelegates::PreBeginPIE.AddRaw(this, &ThisClass::HandleBeginPIE);
	FEditorDelegates::ShutdownPIE.AddRaw(this, &ThisClass::HandleEndPIE);
#else
	CreateMasterAlc();
#endif
}

void FZSharpRuntimeModule::ShutdownModule()
{
	ZSharp::FZUnrealFieldScanner::Get().Shutdown();
	
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
	alc->RegisterZCallResolver(new ZSharp::FZCallResolver_UProperty{}, 2);

	// ZeroGames.ZSharp.UnrealEngine can be accessed via reflection, so we must load it as early as possible.
	alc->LoadAssembly(ZSHARP_ENGINE_ASSEMBLY_NAME);
}

void FZSharpRuntimeModule::UnloadMasterAlc()
{
	ZSharp::IZSharpClr::Get().GetMasterAlc()->Unload();
}

#if WITH_EDITOR
void FZSharpRuntimeModule::HandleBeginPIE(const bool simulating)
{
	CreateMasterAlc();
}

void FZSharpRuntimeModule::HandleEndPIE(const bool simulating)
{
	UnloadMasterAlc();
}
#endif


