﻿// Copyright Zero Games. All Rights Reserved.

#include "ZBuildEngine.h"

#include "IZExportedTypeRegistry.h"
#include "JsonObjectConverter.h"
#include "ZSharpBuildLogChannels.h"
#include "ZSharpExportSettings.h"
#include "ALC/ZCommonMethodArgs.h"
#include "CLR/IZSharpClr.h"
#include "Glue/ZGlueManifestWriter.h"
#include "Interfaces/IPluginManager.h"
#include "Solution/DTO/ZModuleMappingInfoDto.h"

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

	static FString AppendScriptDir(const FString& baseDir)
	{
		return baseDir / "Script";
	}

	static FString BuildArgument(const FString& key, const FString& value)
	{
		return FString::Printf(TEXT("%s=%s"), *key, *value);
	}

	static bool ParseArgument(const TArray<FString>& args, const FString& key, FString& value)
	{
		FRegexPattern pattern { FString::Printf(TEXT("^%s=.+$"), *key) };
		for (const auto& arg : args)
		{
			FRegexMatcher matcher { pattern, arg };
			if (matcher.FindNext())
			{
				arg.Split("=", nullptr, &value);
				return true;
			}
		}

		value = {};
		return false;
	}

	static void ParseArgumentArray(const TArray<FString>& args, const FString& key, TArray<FString>& result)
	{
		FRegexPattern pattern { FString::Printf(TEXT("^%s=.+$"), *key) };
		for (const auto& arg : args)
		{
			FRegexMatcher matcher { pattern, arg };
			if (matcher.FindNext())
			{
				FString value;
				arg.Split("=", nullptr, &value);
				value.ParseIntoArray(result, TEXT(";"));
				break;
			}
		}
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
		{ "Game.zsproj", "Script/Game" },
		{ "DllEntry.cs", "Script/Game/Source" },
		{ "MagicCube.cs", "Script/Game/Source" },
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
		ensure(FFileHelper::SaveStringToFile(content, *dstPath));
	}
}

void ZSharp::FZBuildEngine::GenerateSolution(const TArray<FString>& args) const
{
	const FString projectDir = ZBuildEngine_Private::GetProjectDir();
	const FString pluginDir = ZBuildEngine_Private::GetPluginDir("ZSharp");

	const FString targetArg = ZBuildEngine_Private::BuildArgument("target", "solution");
	const FString projectDirArg = ZBuildEngine_Private::BuildArgument("projectdir", projectDir);
	const FString zsharpDirArg = ZBuildEngine_Private::BuildArgument("zsharpdir", pluginDir);

	FZModuleMappingInfoDto moduleMappingInfoDto;
	GetDefault<UZSharpExportSettings>()->ForeachMappedModule([&moduleMappingInfoDto](const FString& moduleName, const FZModuleMappingContext& ctx)
	{
		moduleMappingInfoDto.Mapping.Emplace(ctx.ModuleName, ctx.AssemblyName);
	});
	FString moduleMappingInfo;
	FJsonObjectConverter::UStructToJsonObjectString(moduleMappingInfoDto, moduleMappingInfo);
	const FString moduleMappingInfoArg = ZBuildEngine_Private::BuildArgument("modulemappinginfo", moduleMappingInfo);

	TArray<FString> sourceDirs = { ZBuildEngine_Private::AppendScriptDir(projectDir), ZBuildEngine_Private::AppendScriptDir(pluginDir) };
#if WITH_EDITOR
	for (const auto& plugin : IPluginManager::Get().GetEnabledPlugins())
	{
		if (plugin->GetName() == "ZSharp")
		{
			continue;
		}
		
		bool hasZSharpScript = false;
		if (plugin->GetDescriptorJson()->TryGetBoolField(TEXT("HasZSharpScript"), hasZSharpScript) && hasZSharpScript)
		{
			sourceDirs.Emplace(ZBuildEngine_Private::AppendScriptDir(ZBuildEngine_Private::GetPluginDir(plugin->GetName())));
		}
	}
#endif
	const FString sourceArg = ZBuildEngine_Private::BuildArgument("source", FString::Join(sourceDirs, TEXT(";")));
	
	TArray<FString> precompiledProjects;
	ZBuildEngine_Private::ParseArgumentArray(args, "prec", precompiledProjects);
	
	FString mode;
	if (!ZBuildEngine_Private::ParseArgument(args, "mode", mode))
	{
		mode = "u";
	}
	
	// Use precompiled Z# assemblies for user mode.
	if (mode == "u" || mode == "u1" || mode == "u2")
	{
		precompiledProjects.AddUnique(ZSHARP_CORE_ASSEMBLY_NAME);
		precompiledProjects.AddUnique(ZSHARP_RESOLVER_ASSEMBLY_NAME);
		precompiledProjects.AddUnique(ZSHARP_CORE_ENGINE_ASSEMBLY_NAME);
		precompiledProjects.AddUnique(ZSHARP_CORE_ASYNC_ASSEMBLY_NAME);
		precompiledProjects.AddUnique("ZeroGames.ZSharp.Emit");
		precompiledProjects.AddUnique(ZSHARP_SCANNER_ASSEMBLY_NAME);
		precompiledProjects.AddUnique(ZSHARP_BUILD_ASSEMBLY_NAME);
		precompiledProjects.AddUnique("ZeroGames.ZSharp.CodeDom.CSharp");

		// Use precompiled engine assembly for user mode level 2.
		if (mode == "u2")
		{
			precompiledProjects.AddUnique(ZSHARP_ENGINE_ASSEMBLY_NAME);
		}
	}
	else if (mode != "dev")
	{
		UE_LOG(LogZSharpBuild, Warning, TEXT("Unknown solution mode [%s]"), *mode);
		return;
	}

	const FString precArg = ZBuildEngine_Private::BuildArgument("prec", FString::Join(precompiledProjects, TEXT(";")));

	const TCHAR* argv[] =
	{
		*targetArg,
		*projectDirArg,
		*zsharpDirArg,
		*sourceArg,
		*moduleMappingInfoArg,
		*precArg,
	};
	FZCommonMethodArgs commonArgs { UE_ARRAY_COUNT(argv), argv };
	IZSharpClr::Get().Run(ZSHARP_BUILD_ASSEMBLY_NAME, &commonArgs);
}

void ZSharp::FZBuildEngine::GenerateGlue(const TArray<FString>& args) const
{
	{ // Step I: Export types and generate manifest.json file.
		TArray<FString> assemblies;
		ZBuildEngine_Private::ParseArgumentArray(args, "asm", assemblies);

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


