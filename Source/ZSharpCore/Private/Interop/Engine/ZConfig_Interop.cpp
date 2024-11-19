// Copyright Zero Games. All Rights Reserved.

#include "ZConfig_Interop.h"

void ZSharp::FZConfig_Interop::GetFileName(FConfigCacheIni* config, const TCHAR* baseIniName, FString& fileName)
{
	fileName = config->GetConfigFilename(baseIniName);
}

uint8 ZSharp::FZConfig_Interop::TryGetSection(FConfigCacheIni* config, const TCHAR* fileName, const TCHAR* section, TArray<FString>& values)
{
	return config->GetSection(section, values, fileName);
}

uint8 ZSharp::FZConfig_Interop::TryGetArray(FConfigCacheIni* config, const TCHAR* fileName, const TCHAR* section, const TCHAR* key, TArray<FString>& values)
{
	return !!config->GetArray(section, key, values, fileName);
}

uint8 ZSharp::FZConfig_Interop::TryGetString(FConfigCacheIni* config, const TCHAR* fileName, const TCHAR* section, const TCHAR* key, FString& value)
{
	return config->GetString(section, key, value, fileName);
}


