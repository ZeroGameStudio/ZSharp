// Copyright Zero Games. All Rights Reserved.

#include "ZSharpExportModule.h"

#include "ZExportedTypeRegistry.h"

class FZSharpExportModule : public IZSharpExportModule
{
	using ThisClass = FZSharpExportModule;
	
	// Begin IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// End IModuleInterface

	void HandlePostEngineInit() const;

	FDelegateHandle PostEngineInitDelegate;
};

IMPLEMENT_MODULE(FZSharpExportModule, ZSharpExport)

void FZSharpExportModule::StartupModule()
{
	PostEngineInitDelegate = FCoreDelegates::OnPostEngineInit.AddRaw(this, &ThisClass::HandlePostEngineInit);
}

void FZSharpExportModule::ShutdownModule()
{
	FCoreDelegates::OnPostEngineInit.Remove(PostEngineInitDelegate);
}

void FZSharpExportModule::HandlePostEngineInit() const
{
	ZSharp::FZExportedTypeRegistry::Get().FinishRegister();
}


