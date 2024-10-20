// Copyright Zero Games. All Rights Reserved.

#include "ZSharpRuntimeModule.h"

#include "ZSharpRuntimeLogChannels.h"
#include "ZSharpRuntimeSettings.h"
#include "CLR/IZSharpClr.h"
#include "ALC/IZMasterAssemblyLoadContext.h"
#include "ALC/ZCommonMethodArgs.h"
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

	void HandlePreMasterAlcStartup(ZSharp::IZMasterAssemblyLoadContext* alc);
	void HandleMasterAlcStartup(ZSharp::IZMasterAssemblyLoadContext* alc);

#if WITH_EDITOR
	void HandleBeginPIE(const bool simulating);
	void HandleEndPIE(const bool simulating);
#endif
	
};

IMPLEMENT_MODULE(FZSharpRuntimeModule, ZSharpRuntime)

void FZSharpRuntimeModule::StartupModule()
{
	ZSharp::IZSharpClr::Get().PreMasterAlcStartup().AddRaw(this, &ThisClass::HandlePreMasterAlcStartup);
	ZSharp::IZSharpClr::Get().OnMasterAlcStartup().AddRaw(this, &ThisClass::HandleMasterAlcStartup);
	
	ZSharp::FZUnrealFieldScanner::Get().Startup();
	
#if WITH_EDITOR
	if (!GIsEditor)
	{
		UE_LOG(LogZSharpRuntime, Log, TEXT("Editor standalone process detected. Will create master ALC immediately."));
		CreateMasterAlc();
	}
	else
	{
		FEditorDelegates::PreBeginPIE.AddRaw(this, &ThisClass::HandleBeginPIE);
		FEditorDelegates::EndPIE.AddRaw(this, &ThisClass::HandleEndPIE);
	}
#else
	CreateMasterAlc();
#endif
}

void FZSharpRuntimeModule::ShutdownModule()
{
	ZSharp::FZUnrealFieldScanner::Get().Shutdown();
	
#if WITH_EDITOR
	if (!GIsEditor)
    {
    	UnloadMasterAlc();
    }
    else
    {
    	FEditorDelegates::PreBeginPIE.RemoveAll(this);
    	FEditorDelegates::EndPIE.RemoveAll(this);
    }
#else
	UnloadMasterAlc();
#endif

	ZSharp::IZSharpClr::Get().PreMasterAlcStartup().RemoveAll(this);
	ZSharp::IZSharpClr::Get().OnMasterAlcStartup().RemoveAll(this);
}

void FZSharpRuntimeModule::CreateMasterAlc()
{
	ZSharp::IZSharpClr::Get().CreateMasterAlc();
}

void FZSharpRuntimeModule::UnloadMasterAlc()
{
	ZSharp::IZSharpClr::Get().GetMasterAlc()->Unload();
}

void FZSharpRuntimeModule::HandlePreMasterAlcStartup(ZSharp::IZMasterAssemblyLoadContext* alc)
{
	alc->RegisterZCallResolver(new ZSharp::FZCallResolver_Export{}, 0);
	alc->RegisterZCallResolver(new ZSharp::FZCallResolver_UFunction{}, 1);
	alc->RegisterZCallResolver(new ZSharp::FZCallResolver_UProperty{}, 2);

	// ZeroGames.ZSharp.UnrealEngine can be accessed via reflection, so we must load it as early as possible.
	alc->LoadAssembly(ZSHARP_ENGINE_ASSEMBLY_NAME);
}

void FZSharpRuntimeModule::HandleMasterAlcStartup(ZSharp::IZMasterAssemblyLoadContext* alc)
{
	GetDefault<UZSharpRuntimeSettings>()->ForeachMasterAlcStartupAssembly([alc](const FZMasterAlcStartupAssembly& assembly)
	{
		TArray<const TCHAR*> argv;
		for (const auto& arg : assembly.Arguments)
		{
			argv.Emplace(*arg);
		}
			
		ZSharp::FZCommonMethodArgs commonArgs = { argv.Num(), argv.GetData() };
		alc->LoadAssembly(assembly.AssemblyName, &commonArgs);
	});
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


