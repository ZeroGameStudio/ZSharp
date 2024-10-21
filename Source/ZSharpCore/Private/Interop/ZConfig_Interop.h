// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	struct FZConfig_Interop
	{
		static void GetFileName(FConfigCacheIni* config, const TCHAR* baseIniName, FString& fileName);
		static uint8 TryGetSection(FConfigCacheIni* config, const TCHAR* fileName, const TCHAR* section, FString& values);
		static uint8 TryGetArray(FConfigCacheIni* config, const TCHAR* fileName, const TCHAR* section, const TCHAR* key, FString& values);
		static uint8 TryGetString(FConfigCacheIni* config, const TCHAR* fileName, const TCHAR* section, const TCHAR* key, FString& value);
	};
}


