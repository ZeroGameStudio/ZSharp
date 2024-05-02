// Copyright Zero Games. All Rights Reserved.

#include "ZSharpEditorModule.h"

#include "ALC/ZCommonDllMainArgs.h"
#include "CLR/IZSharpClr.h"
#include "Interfaces/IPluginManager.h"

namespace ZSharp::FZSharpEditorModule_Private
{
	static FAutoConsoleCommand GCmdDotnetRun(
		TEXT("zsharp.Gen"),
		TEXT("Generate zsharp solution and project files."),
		FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& args)
		{
			const FString pluginDir = IPluginManager::Get().GetModuleOwnerPlugin(UE_MODULE_NAME)->GetBaseDir();
			
			FString dllPath = FPaths::Combine(FPaths::ProjectDir(), "Binaries/Managed/ZeroGames.ZSharp.Build.dll");
			if (!FPaths::FileExists(dllPath))
			{
				dllPath = FPaths::Combine(pluginDir, "Content/ZeroGames.ZSharp.Build.dll");
				check(FPaths::FileExists(dllPath));
			}

			FString projectDirArg = FString::Printf(TEXT("projectdir=%s"), *FPaths::ConvertRelativePathToFull(FPaths::ProjectDir()));
			FString projectManagedSourceDir = FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectDir(), "Source/Managed"));
			FString pluginManagedSourceDir = FPaths::ConvertRelativePathToFull(FPaths::Combine(pluginDir, "Source/Managed"));
			FString sourceArg = FString::Printf(TEXT("source=%s"), *FString::Printf(TEXT("%s;%s"), *projectManagedSourceDir, *pluginManagedSourceDir));
			const TCHAR* argv[] =
			{
				TEXT("target=1"),
				*projectDirArg,
				*sourceArg,
			};
			FZCommonDllMainArgs commonArgs { UE_ARRAY_COUNT(argv), argv };
			IZSharpClr::Get().Run(dllPath, &commonArgs);
		}),
		ECVF_Default);
}

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
