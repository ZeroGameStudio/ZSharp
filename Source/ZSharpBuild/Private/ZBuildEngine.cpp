// Copyright Zero Games. All Rights Reserved.


#include "ZBuildEngine.h"

#include "ALC/ZCommonDllMainArgs.h"
#include "CLR/IZSharpClr.h"
#include "Glue/ZDynamicTypeExporter.h"
#include "Glue/ZGlueManifestWriter.h"
#include "Interfaces/IPluginManager.h"
#include "Static/ZStaticExportedEnum.h"

// @TODO: Remove
ZSHARP_DECLARE_EXPORTED_ENUM(EForceInit, EForceInit, Core)

ZSHARP_BEGIN_EXPORT_ENUM(EForceInit, false)
	ZSHARP_EXPORT_ENUM_VALUE(ForceInit)
	ZSHARP_EXPORT_ENUM_VALUE(ForceInitToZero)
ZSHARP_END_EXPORT_ENUM(EForceInit)

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
	
	static FString LocateBuildAssembly()
	{
		const FString pluginDir = IPluginManager::Get().GetModuleOwnerPlugin(UE_MODULE_NAME)->GetBaseDir();
	
		FString dllPath = FPaths::Combine(FPaths::ProjectDir(), "Binaries/Managed/ZeroGames.ZSharp.Build.dll");
		if (!FPaths::FileExists(dllPath))
		{
			dllPath = FPaths::Combine(pluginDir, "Content/ZeroGames.ZSharp.Build.dll");
		}

		check(FPaths::FileExists(dllPath));

		return dllPath;
	}

	static FString GetProjectDir()
	{
		return FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
	}

	static FString GetPluginDir()
	{
		return FPaths::ConvertRelativePathToFull(IPluginManager::Get().GetModuleOwnerPlugin(UE_MODULE_NAME)->GetBaseDir());
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
	const FString assemblyDir = ZBuildEngine_Private::LocateBuildAssembly();
	const FString projectDir = ZBuildEngine_Private::GetProjectDir();
	const FString pluginDir = ZBuildEngine_Private::GetPluginDir();

	const FString targetArg = ZBuildEngine_Private::BuildArgument("target", "solution");
	const FString projectDirArg = ZBuildEngine_Private::BuildArgument("projectdir", projectDir);
	const FString projectManagedSourceDir = FPaths::Combine(projectDir, "Source/Managed");
	const FString pluginManagedSourceDir = FPaths::Combine(pluginDir, "Source/Managed");
	const FString sourceArg = ZBuildEngine_Private::BuildArgument("source", FString::Printf(TEXT("%s;%s"), *projectManagedSourceDir, *pluginManagedSourceDir));

	const TCHAR* argv[] =
	{
		*targetArg,
		*projectDirArg,
		*sourceArg,
	};
	FZCommonDllMainArgs commonArgs { UE_ARRAY_COUNT(argv), argv };
	IZSharpClr::Get().Run(assemblyDir, &commonArgs);
}

void ZSharp::FZBuildEngine::GenerateGlue() const
{
	FZDynamicTypeExporter{}.Export();
	FZGlueManifestWriter{}.Write();

	const FString assemblyDir = ZBuildEngine_Private::LocateBuildAssembly();
	const FString projectDir = ZBuildEngine_Private::GetProjectDir();

	const FString targetArg = ZBuildEngine_Private::BuildArgument("target", "glue");
	const FString projectDirArg = ZBuildEngine_Private::BuildArgument("projectdir", projectDir);

	const TCHAR* argv[] =
	{
		*targetArg,
		*projectDirArg,
	};
	FZCommonDllMainArgs commonArgs { UE_ARRAY_COUNT(argv), argv };
	IZSharpClr::Get().Run(assemblyDir, &commonArgs);
}

void ZSharp::FZBuildEngine::BuildSolution() const
{

}


