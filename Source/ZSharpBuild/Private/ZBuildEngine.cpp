// Copyright Zero Games. All Rights Reserved.


#include "ZBuildEngine.h"

#include "ZSharpBuildSettings.h"
#include "ALC/ZCommonMethodArgs.h"
#include "CLR/IZSharpClr.h"
#include "Glue/ZDynamicTypeExporter.h"
#include "Glue/ZGlueManifestWriter.h"
#include "Interfaces/IPluginManager.h"

namespace ZSharp::ZBuildEngine_Private
{
	static FAutoConsoleCommand GCmdZSharpGens(
		TEXT("zs.gens"),
		TEXT("Generate ZSharp solution and project files."),
		FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& args)
		{
			FZBuildEngine::Get().GenerateSolution();
		}),
		ECVF_Default);

	static FAutoConsoleCommand GCmdZSharpGeng(
		TEXT("zs.geng"),
		TEXT("Generate ZSharp glue code."),
		FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& args)
		{
			FZBuildEngine::Get().GenerateGlue();
		}),
		ECVF_Default);

	static FString GetProjectDir()
	{
		return FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
	}

	static FString GetPluginDir(const FString& pluginName)
	{
		TSharedPtr<IPlugin> plugin = IPluginManager::Get().FindEnabledPlugin(*pluginName);
		if (!plugin)
		{
			return {};
		}
		
		return FPaths::ConvertRelativePathToFull(plugin->GetBaseDir());
	}

	static FString BuildArgument(const FString& key, const FString& value)
	{
		return FString::Printf(TEXT("%s=%s"), *key, *value);
	}
}

ZSharp::FZBuildEngine& ZSharp::FZBuildEngine::Get()
{
	static FZBuildEngine GSingleton;

	return GSingleton;
}

void ZSharp::FZBuildEngine::GenerateSolution() const
{
	const FString projectDir = ZBuildEngine_Private::GetProjectDir();
	const FString pluginDir = ZBuildEngine_Private::GetPluginDir("ZSharp");

	const FString targetArg = ZBuildEngine_Private::BuildArgument("target", "solution");
	const FString projectDirArg = ZBuildEngine_Private::BuildArgument("projectdir", projectDir);
	const FString projectManagedSourceDir = FPaths::Combine(projectDir, "Source/Managed");
	const FString pluginManagedSourceDir = FPaths::Combine(pluginDir, "Source/Managed");
	FString dirsToScan = FString::Printf(TEXT("%s;%s"), *projectManagedSourceDir, *pluginManagedSourceDir);
	for (const auto& pluginName : GetDefault<UZSharpBuildSettings>()->GetPluginsToScanZsproj())
	{
		FString dir = ZBuildEngine_Private::GetPluginDir(pluginName);
		dir = FPaths::Combine(dir, "Source/Managed");
		if (dir.Len())
		{
			dirsToScan.Append(";").Append(dir);
		}
	}
	const FString sourceArg = ZBuildEngine_Private::BuildArgument("source", dirsToScan);

	const TCHAR* argv[] =
	{
		*targetArg,
		*projectDirArg,
		*sourceArg,
	};
	FZCommonMethodArgs commonArgs { UE_ARRAY_COUNT(argv), argv };
	IZSharpClr::Get().Run(ZSHARP_BUILD_ASSEMBLY_NAME, &commonArgs);
}

void ZSharp::FZBuildEngine::GenerateGlue() const
{
	FZDynamicTypeExporter{}.Export();
	FZGlueManifestWriter{}.Write();

	const FString projectDir = ZBuildEngine_Private::GetProjectDir();

	const FString targetArg = ZBuildEngine_Private::BuildArgument("target", "glue");
	const FString projectDirArg = ZBuildEngine_Private::BuildArgument("projectdir", projectDir);

	const TCHAR* argv[] =
	{
		*targetArg,
		*projectDirArg,
	};
	FZCommonMethodArgs commonArgs { UE_ARRAY_COUNT(argv), argv };
	IZSharpClr::Get().Run(ZSHARP_BUILD_ASSEMBLY_NAME, &commonArgs);
}

void ZSharp::FZBuildEngine::BuildSolution() const
{

}


