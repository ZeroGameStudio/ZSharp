// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct FZPath_Interop
	{
		static void GetProjectDir(FString& outDir);
		static void GetPluginDir(const TCHAR* pluginName, FString& outDir);
	};
}


