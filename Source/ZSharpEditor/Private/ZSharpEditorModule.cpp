// Copyright Zero Games. All Rights Reserved.

#include "ZSharpEditorModule.h"

#include "IZBuildEngine.h"
#include "Interfaces/IMainFrameModule.h"

namespace ZSharp::ZSharpEditorModule_Private
{
	static void Generate()
	{
#define LOCTEXT_NAMESPACE "ZSharpProjectStartup"

		const FString sourceDir = FPaths::ProjectDir() / "Source";
		if (!FPaths::DirectoryExists(sourceDir))
		{
			return;
		}
	
		const FString scriptDir = FPaths::ProjectDir() / "Script";
		const FString DefaultZSharpIniPath = FPaths::ProjectConfigDir() / "DefaultZSharp.ini";
		if (FPaths::DirectoryExists(scriptDir) || FPaths::FileExists(DefaultZSharpIniPath))
		{
			return;
		}

		static const FText GWelcomeTitle = LOCTEXT("WelcomeTitle", "Welcome to Z#");
		static const FText GWelcomeMessage = LOCTEXT("WelcomeMessage", "This is the first time you run unreal editor with Z#. Would you like to have some example content?");

		if (FMessageDialog::Open(EAppMsgCategory::Success, EAppMsgType::YesNo, GWelcomeMessage, GWelcomeTitle) != EAppReturnType::Yes)
		{
			return;
		}

		{
			FScopedSlowTask slowTask(100, LOCTEXT("GeneratingExampleContent", "Generating example content..."));
			slowTask.MakeDialog();
	
			slowTask.EnterProgressFrame(25, LOCTEXT("GeneratingExampleContent", "Generating example content..."));
			ZSharp::IZBuildEngine::Get().GenerateExampleContent();

			slowTask.EnterProgressFrame(25, LOCTEXT("GeneratingExampleContent", "Generating Visual Studio project files..."));
			ZSharp::IZBuildEngine::Get().GenerateSolution({});

			slowTask.EnterProgressFrame(50, LOCTEXT("GeneratingExampleContent", "Generating glue code..."));
			ZSharp::IZBuildEngine::Get().GenerateGlue({});
		}

		static const FText GRestartTitle = LOCTEXT("RestartTitle", "Generate Finished");
		static const FText GRestartMessage = LOCTEXT("RestartMessage", "Example content generated:\n\n/Script/*\n/Config/DefaultZSharp.ini\n\nThe editor will close. Please build the Script.sln in project root directory then restart the editor.");

		FMessageDialog::Open(EAppMsgCategory::Success, EAppMsgType::Ok, GRestartMessage, GRestartTitle);
		RequestEngineExit(TEXT("Z# Startup"));

#undef LOCTEXT_NAMESPACE
	}
}

class FZSharpEditorModule : public IZSharpEditorModule
{
	using ThisClass = FZSharpEditorModule;
	
	// Begin IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// End IModuleInterface

	void HandleMainFrameCreationFinished(TSharedPtr<SWindow>, bool);
};

IMPLEMENT_MODULE(FZSharpEditorModule, ZSharpEditor)

void FZSharpEditorModule::StartupModule()
{
	IMainFrameModule& MainFrameModule = FModuleManager::LoadModuleChecked<IMainFrameModule>(TEXT("MainFrame"));
	MainFrameModule.OnMainFrameCreationFinished().AddRaw(this, &ThisClass::HandleMainFrameCreationFinished);
}

void FZSharpEditorModule::ShutdownModule()
{
	IMainFrameModule& MainFrameModule = FModuleManager::LoadModuleChecked<IMainFrameModule>(TEXT("MainFrame"));
	MainFrameModule.OnMainFrameCreationFinished().RemoveAll(this);
}

void FZSharpEditorModule::HandleMainFrameCreationFinished(TSharedPtr<SWindow>, bool)
{
	ExecuteOnGameThread(TEXT("Generate Z# Example Content"), []
	{
		if (IsEngineExitRequested())
		{
			return;
		}
		
		ZSharp::ZSharpEditorModule_Private::Generate();
	});
}


