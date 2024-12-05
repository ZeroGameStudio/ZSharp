// Copyright Zero Games. All Rights Reserved.


#include "ZBuildEngine.h"

#include "IZExportedTypeRegistry.h"
#include "ALC/ZCommonMethodArgs.h"
#include "CLR/IZSharpClr.h"
#include "Glue/ZGlueManifestWriter.h"
#include "Interfaces/IPluginManager.h"

namespace ZSharp::ZBuildEngine_Private
{
	static FAutoConsoleCommand GCmdZSharpGens(
		TEXT("z#.gens"),
		TEXT("Generate ZSharp solution and project files."),
		FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& args)
		{
			FZBuildEngine::Get().GenerateSolution(args);
		}),
		ECVF_Default);

	static FAutoConsoleCommand GCmdZSharpGeng(
		TEXT("z#.geng"),
		TEXT("Generate ZSharp glue code."),
		FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& args)
		{
			FZBuildEngine::Get().GenerateGlue(args);
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

	static FString AppendSourceManagedDir(const FString& baseDir)
	{
		return baseDir / "Source" / "Managed";
	}

	static FString BuildArgument(const FString& key, const FString& value)
	{
		return FString::Printf(TEXT("%s=%s"), *key, *value);
	}
}

ZSharp::FZBuildEngine& ZSharp::FZBuildEngine::Get()
{
	static FZBuildEngine GInstance;

	return GInstance;
}

void ZSharp::FZBuildEngine::GenerateExampleContent() const
{
	FString gameModuleName;
	TArray<FModuleStatus> statuses;
	FModuleManager::Get().QueryModules(statuses);
	for (const auto& status : statuses)
	{
		if (status.bIsGameModule)
		{
			gameModuleName = status.Name;
			break;
		}
	}

	if (!ensure(!gameModuleName.IsEmpty()))
	{
		return;
	}

	auto replaceWildcards = [gameModuleName](FString& text)
	{
		text.ReplaceInline(TEXT("${GameModuleName}"), *gameModuleName);
	};

	const TMap<FString, FString> templateTargetDirMap
	{
		{ "DefaultZSharp.ini", "Config" },
		{ "Game.zsproj", "Source/Managed/Game" },
		{ "DllEntry.cs", "Source/Managed/Game/Source" },
		{ "MagicCube.cs", FString::Printf(TEXT("Source/Managed/Game/Source/%s"), *gameModuleName) },
	};

	const FString projectDir = ZBuildEngine_Private::GetProjectDir();
	const FString pluginDir = ZBuildEngine_Private::GetPluginDir("ZSharp");
	const FString pluginTemplateDir = pluginDir / "Template";

	for (const auto& pair : templateTargetDirMap)
	{
		const FString srcPath = pluginTemplateDir / pair.Key;
		const FString dstPath = projectDir / pair.Value / pair.Key;
		FString content;
		ensure(FFileHelper::LoadFileToString(content, *srcPath));
		replaceWildcards(content);
		UE_LOG(LogTemp, Log, TEXT("%s"), *content);
		//ensure(FFileHelper::SaveStringToFile(content, *dstPath));
	}
}

void ZSharp::FZBuildEngine::GenerateSolution(const TArray<FString>& args) const
{
	const FString projectDir = ZBuildEngine_Private::GetProjectDir();
	const FString pluginDir = ZBuildEngine_Private::GetPluginDir("ZSharp");

	const FString targetArg = ZBuildEngine_Private::BuildArgument("target", "solution");
	const FString projectDirArg = ZBuildEngine_Private::BuildArgument("projectdir", projectDir);
	const FString zsharpDirArg = ZBuildEngine_Private::BuildArgument("zsharpdir", pluginDir);

	TArray<FString> sourceDirs = { ZBuildEngine_Private::AppendSourceManagedDir(projectDir), ZBuildEngine_Private::AppendSourceManagedDir(pluginDir) };
#if WITH_EDITOR
	for (const auto& plugin : IPluginManager::Get().GetEnabledPlugins())
	{
		if (plugin->GetName() == "ZSharp")
		{
			continue;
		}
		
		bool hasZSharpProject = false;
		if (plugin->GetDescriptorJson()->TryGetBoolField(TEXT("HasZSharpProject"), hasZSharpProject) && hasZSharpProject)
		{
			sourceDirs.Emplace(ZBuildEngine_Private::AppendSourceManagedDir(ZBuildEngine_Private::GetPluginDir(plugin->GetName())));
		}
	}
#endif
	const FString sourceArg = ZBuildEngine_Private::BuildArgument("source", FString::Join(sourceDirs, TEXT(";")));

	const TCHAR* argv[] =
	{
		*targetArg,
		*projectDirArg,
		*zsharpDirArg,
		*sourceArg,
	};
	FZCommonMethodArgs commonArgs { UE_ARRAY_COUNT(argv), argv };
	IZSharpClr::Get().Run(ZSHARP_BUILD_ASSEMBLY_NAME, &commonArgs);
}

void ZSharp::FZBuildEngine::GenerateGlue(const TArray<FString>& args) const
{
	{ // Step I: Export types and generate manifest.json file.
		TArray<FString> assemblies;
		FRegexPattern pattern { "^assemblies=.+$" };
		for (const auto& arg : args)
		{
			FRegexMatcher matcher { pattern, arg };
			if (matcher.FindNext())
			{
				FString value;
				arg.Split("=", nullptr, &value);
				value.ParseIntoArray(assemblies, TEXT(","));
			}
		}

		IZExportedTypeRegistry::Get().ExportDynamicTypes(assemblies);
		FZGlueManifestWriter{}.Write(assemblies);
	}
	
	{ // Step II: Call ZeroGames.ZSharp.Build to generate glue code.
		const FString projectDir = ZBuildEngine_Private::GetProjectDir();

		TArray<FString> stringArgs;
		stringArgs.Emplace(ZBuildEngine_Private::BuildArgument("target", "glue"));
		stringArgs.Emplace(ZBuildEngine_Private::BuildArgument("projectdir", projectDir));

		FRegexPattern pattern { "^.+=.+$" };
		for (const auto& arg : args)
		{
			FRegexMatcher matcher { pattern, arg };
			if (matcher.FindNext())
			{
				stringArgs.Emplace(arg);
			}
		}

		TArray<const TCHAR*> rawArgs;
		for (const auto& arg : stringArgs)
		{
			rawArgs.Emplace(*arg);
		}
	
		FZCommonMethodArgs commonArgs { rawArgs.Num(), rawArgs.GetData() };
		IZSharpClr::Get().Run(ZSHARP_BUILD_ASSEMBLY_NAME, &commonArgs);
	}
}

void ZSharp::FZBuildEngine::BuildSolution(const TArray<FString>& args) const
{

}


