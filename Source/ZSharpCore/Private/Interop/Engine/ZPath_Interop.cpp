// Copyright Zero Games. All Rights Reserved.


#include "ZPath_Interop.h"

#include "Interfaces/IPluginManager.h"

void ZSharp::FZPath_Interop::GetProjectDir(FString& outDir)
{
	outDir = FPaths::ProjectDir();
}

void ZSharp::FZPath_Interop::GetPluginDir(const TCHAR* pluginName, FString& outDir)
{
	outDir = IPluginManager::Get().FindEnabledPlugin(pluginName)->GetBaseDir();
}


