// Copyright Zero Games. All Rights Reserved.

#include "ZSharpEditorModule.h"

class FZSharpEditorModule : public IZSharpEditorModule
{
	// Begin IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// End IModuleInterface
};

IMPLEMENT_MODULE(FZSharpEditorModule, ZSharpEditor)

void FZSharpEditorModule::StartupModule()
{
}

void FZSharpEditorModule::ShutdownModule()
{
}
