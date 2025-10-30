// Copyright Zero Games. All Rights Reserved.

#include "ZPath_Interop.h"

#include "Interfaces/IPluginManager.h"
#include "Interop/ZInteropExceptionHelper.h"

namespace ZSharp::ZPath_Interop_Private
{
	static void GetProjectDir(FString& outDir)
	{
		outDir = FPaths::ProjectDir();
	}

	static void GetPluginDir(const TCHAR* pluginName, FString& outDir)
	{
		outDir = IPluginManager::Get().FindEnabledPlugin(pluginName)->GetBaseDir();
	}
}

void ZSharp::FZPath_Interop::GetProjectDir(FString& outDir)
{
	GUARD(ZPath_Interop_Private::GetProjectDir(outDir));
}

void ZSharp::FZPath_Interop::GetPluginDir(const TCHAR* pluginName, FString& outDir)
{
	GUARD(ZPath_Interop_Private::GetPluginDir(pluginName, outDir));
}


